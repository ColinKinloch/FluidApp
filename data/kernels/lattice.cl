// OpenCL LBM
// tau = Relaxation time τ
// rho = Input density ρ
// x = Horizontal position of current node
// y = Verticle position of current node
// coord = Coordinate of current node
// scoord = Current screen coodinate
// i = Index of current nodes rest speed
// si = Screen index of the current node
// D = Number of dimentions in current LBM
// Q = Number of speeds in current LBM
// l* = Local cache of speeds for current node
// eq* = Local equilibrium value
// 5 2 6
//  \|/ 
// 1-0-3
//  /|\ 
// 8 4 7
// Colin Kinloch - 1026970

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#define D 2
#define Q 9

#define LTYPE float

#define CSID(x, y, width) (y*width+x)
#define CID(x, y, width) Q*CSID(x, y, width)
// Returns screen index for coordinate
#define SID(coord, width) ( coord.y*width+coord.x )
// Returns rest velocity index for node at coordinatie
#define ID(coord, width) Q*SID(coord, width)

// Move speeds in lattice direction
__kernel void stream(__global __read_only LTYPE* oldLattice, __global __write_only LTYPE* newLattice, int width, int height, float dt)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	
        unsigned int i = ID(coord, width);
	// Coordinates for adjacent nodes
	int xl = x-1;
	int yl = y-1;
	int xh = x+1;
	int yh = y+1;
	
	// Edge cases
	if(x==0) xl=0;
	if(y==0) yl=0;
	if(x==(width -1)) xh=width -1;
	if(y==(height-1)) yh=height-1;
	newLattice[i  ] = oldLattice[i];
        newLattice[i+1] = oldLattice[CID(xl, y , width)+1];
        newLattice[i+2] = oldLattice[CID(x , yl, width)+2];
        newLattice[i+3] = oldLattice[CID(xh, y , width)+3];
        newLattice[i+4] = oldLattice[CID(x , yh, width)+4];
        newLattice[i+5] = oldLattice[CID(xl, yl, width)+5];
        newLattice[i+6] = oldLattice[CID(xh, yl, width)+6];
        newLattice[i+7] = oldLattice[CID(xh, yh, width)+7];
        newLattice[i+8] = oldLattice[CID(xl, yh, width)+8];
}

// BGK collision opperator
__kernel void collide(__global LTYPE* lattice, __global float* vMags, int width, float tau)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	
        unsigned int si = SID(coord, width);
	unsigned int i = si*Q;
	
	// Weighting for rest, horizontal/vertical and diagonals
        float w0 = 4.f/9.f;
	float w1 = 1.f/9.f;
	float w2 = 1.f/36.f;
	
	float atau = 1.f/tau;
	float atau1 = 1.f-atau;
	
	LTYPE l[Q];
	float rho, rhovx, rhovy;
	float vx, vy;
	float vSq;
	
	// Local lattice node filled and node density calculated
	for(int q=0; q<Q; ++q)
	 rho += l[q] = lattice[i+q];
	
	rhovx = l[1]-l[3]+l[5]-l[6]-l[7]+l[8];
	rhovy = l[2]-l[4]+l[5]+l[6]-l[7]-l[8];
	
	vx = rhovx/rho;
	vy = rhovy/rho;
	
	// Speeds stored for use in rendering
	vMags[si] = sqrt(vx*vx+vy*vy);
	
	vSq = 1.5f*(vx*vx + vy*vy);
	
	// Equilibrium values calculated
	LTYPE eq[Q];
	eq[0] = w0*rho*(1.f																				-vSq);
	eq[1] = w1*rho*(1.f+3.f*vx				+4.5f*vx*vx							-vSq);
	eq[2] = w1*rho*(1.f+3.f*vy				+4.5f*vy*vy							-vSq);
	eq[3] = w1*rho*(1.f-3.f*vx				+4.5f*vx*vx							-vSq);
	eq[4] = w1*rho*(1.f-3.f*vy				+4.5f*vy*vy							-vSq);
	eq[5] = w2*rho*(1.f+3.f*( vx+vy)	+4.5f*( vx+vy)*( vx+vy)	-vSq);
	eq[6] = w2*rho*(1.f+3.f*(-vx+vy)	+4.5f*(-vx+vy)*(-vx+vy)	-vSq);
	eq[7] = w2*rho*(1.f+3.f*(-vx-vy)	+4.5f*(-vx-vy)*(-vx-vy)	-vSq);
	eq[8] = w2*rho*(1.f+3.f*( vx-vy)	+4.5f*( vx-vy)*( vx-vy)	-vSq);
	
	// Particle densities returned to the global lattice
	for(int q = 0; q<Q; ++q)
	 lattice[i+q] = atau1*l[q]+atau*eq[q];
}

// Bounce back from solid surfaces
__kernel void solidBB(__global LTYPE* lattice, __global char* __read_only solid, int width)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	
        unsigned int si = SID(coord, width);
	unsigned int i = si*Q;
	
	LTYPE l[Q];
	
	
	if(solid[si])
	{
		for(int q=1; q<Q; ++q)
		 l[q] = lattice[i+q];
		
		lattice[i+1] = l[3];
		lattice[i+2] = l[4];
		lattice[i+3] = l[1];
		lattice[i+4] = l[2];
		lattice[i+5] = l[7];
		lattice[i+6] = l[8];
		lattice[i+7] = l[5];
		lattice[i+8] = l[6];
	}
}

// Wraps particles vertically
__kernel void vWrap(__global LTYPE* lattice, int width, int height)
{
	int x = get_global_id(0);
	
        unsigned int ib = CID(x, 0, width);
        unsigned int it = CID(x, height-1, width);
	//offtop
	lattice[ib+2] = lattice[it+2];
	lattice[ib+5] = lattice[it+5];
	lattice[ib+6] = lattice[it+6];
	
	//offbottom
	lattice[it+4] = lattice[ib+4];
	lattice[it+7] = lattice[ib+7];
	lattice[it+8] = lattice[ib+8];
}

// Wraps particles horizontal
__kernel void hWrap(__global LTYPE* lattice, int width, int height)
{
	int y = get_global_id(0);
	
        unsigned int ir = CID(0, y, width);
        unsigned int il = CID(width-1, y, width);
	//offleft
	lattice[ir+1] = lattice[il+1];
	lattice[ir+5] = lattice[il+5];
	lattice[ir+8] = lattice[il+8];
	
	//offright
	lattice[il+3] = lattice[ir+3];
	lattice[il+7] = lattice[ir+7];
	lattice[il+6] = lattice[ir+6];
}

// Disabled inflow function
__kernel void inflow(__global LTYPE* lattice, float vx, float vy, float rho, int width)
{
	int y = get_global_id(0);
	int2 coord = (int2)(0, y);
	
        unsigned int i = ID(coord, width);
	
	LTYPE l[Q];
	
        //int w0 = 4.f/9.f;
	int w1 = 1.f/9.f;
	int w2 = 1.f/36.f;
	
	LTYPE vSq = 1.5f*(vx*vx+vy*vy);
	
	// Adds initial equilibrium velocities to right facing speeds on left edge nodes
	l[1] = w1*rho*(1.f+3.f*vx				+4.5f*vx*vx							-vSq);
	l[5] = w2*rho*(1.f+3.f*( vx+vy)	+4.5f*( vx+vy)*( vx+vy)	-vSq);
	l[8] = w2*rho*(1.f+3.f*( vx-vy)	+4.5f*( vx-vy)*( vx-vy)	-vSq);
	
	lattice[i+1] = l[1];
	lattice[i+5] = l[5];
	lattice[i+8] = l[8];
}

//Draw velocities
__kernel void oldRender(__global LTYPE* __read_only vMags,
 __global char* __read_only solid, int width, int height, float vx, float vy,
 __write_only image2d_t texture)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	int2 scoord = (int2)(x, height-1-y);
	unsigned int si = SID(coord, width);

        //float minc=0.f;
        //float maxc=0.1f;
        LTYPE mv = sqrt(vx*vx+vy*vy);

	LTYPE v=vMags[si]/mv;
	float r, g, b, a = 1.f;

	// Each colour has a different arbitrary weighting this is purely cosmetic
	r = v*0.25f;
	g = v*0.5f;
	b = 1.f;

	// If the coordinate coincides with a surface
	if(solid[si])
	 r = g = b = 0.f;

	float4 colour = (float4)(r, g, b, a);

	// Write colour to framebuffer
	write_imagef(texture, scoord, colour);
}

__kernel void render(__global LTYPE* __read_only vMags,
 __global char* __read_only solid, int width, int height, float vx, float vy,
 __write_only image2d_t texture)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	int2 scoord = (int2)(x, height-1-y);
	unsigned int si = SID(coord, width);

        //float minc=0.f;
        //float maxc=0.1f;
        LTYPE mv = sqrt(vx*vx+vy*vy);

	LTYPE v=vMags[si]/mv;
	float r, g, b, a = 1.f;

	// Each colour has a different arbitrary weighting this is purely cosmetic
	r = 1-2*v;
	g = 1-2*fabs(v-0.5f);
	b = 2*v-1;

	// If the coordinate coincides with a surface
	if(solid[si])
	 r = g = b = 0.f;

	float4 colour = (float4)(r, g, b, a);

	// Write colour to framebuffer
	write_imagef(texture, scoord, colour);
}

