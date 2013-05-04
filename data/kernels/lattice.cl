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

#define D 2
#define Q 9

// Returns screen index for coordinate
uint getSID(int2 coord, int width)
{
	return coord.y*width+coord.x;
}

// Returns rest velocity index for node at coordinatie
uint getID(int2 coord, int width)
{
	return Q*getSID(coord, width);
}

// Move speeds in lattice direction
__kernel void stream(__global __read_only float* oldLattice, __global __write_only float* newLattice, int width, int height, float dt)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	
	unsigned int i = getID(coord, width);
	
	float l[Q];
	
	int xl = x-1;
	int yl = y-1;
	int xh = x+1;
	int yh = y+1;
	if(x==0) xl=0;
	if(y==0) yl=0;
	if(x==(width -1)) xh=width -1;
	if(y==(height-1)) yh=height-1;
	newLattice[i  ] = oldLattice[i];
	newLattice[i+1] = oldLattice[getID((int2)(xl, y ), width)+1];
	newLattice[i+2] = oldLattice[getID((int2)(x , yl), width)+2];
	newLattice[i+3] = oldLattice[getID((int2)(xh, y ), width)+3];
	newLattice[i+4] = oldLattice[getID((int2)(x , yh), width)+4];
	newLattice[i+5] = oldLattice[getID((int2)(xl, yl), width)+5];
	newLattice[i+6] = oldLattice[getID((int2)(xh, yl), width)+6];
	newLattice[i+7] = oldLattice[getID((int2)(xh, yh), width)+7];
	newLattice[i+8] = oldLattice[getID((int2)(xl, yh), width)+8];
}

__kernel void collide(__global float* lattice, __global float* vMags, int width, float tau)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	
	unsigned int si = getSID(coord, width);
	unsigned int i = si*Q;
	
	float w0 = 4.f/9.f;
	float w1 = 1.f/9.f;
	float w2 = 1.f/36.f;
	
	float atau = 1.f/tau;
	float atau1 = 1.f - atau;
	
	float l[Q];
	float rho, rhovx, rhovy;
	float vx, vy;
	float vSq;
	
	for(int q=0; q<Q; ++q)
	 rho += l[q] = lattice[i+q];
	
	rhovx = l[1] - l[3] + l[5] - l[6] - l[7] + l[8];
	rhovy = l[2] - l[4] + l[5] + l[6] - l[7] - l[8];
	
	vx = rhovx/rho;
	vy = rhovy/rho;
	
	vMags[si] = sqrt(vx*vx+vy*vy);
	
	vSq = 1.5f*(vx*vx + vy*vy);
	
	float eq[Q];
	eq[0] = rho*w0*(1.f																				-vSq);
	eq[1] = rho*w1*(1.f+3.f*vx				+4.5f*vx*vx							-vSq);
	eq[2] = rho*w1*(1.f+3.f*vy				+4.5f*vy*vy							-vSq);
	eq[3] = rho*w1*(1.f-3.f*vx				+4.5f*vx*vx							-vSq);
	eq[4] = rho*w1*(1.f-3.f*vy				+4.5f*vy*vy							-vSq);
	eq[5] = rho*w2*(1.f+3.f*( vx+vy)	+4.5f*( vx+vy)*( vx+vy)	-vSq);
	eq[6] = rho*w2*(1.f+3.f*(-vx+vy)	+4.5f*(-vx+vy)*(-vx+vy)	-vSq);
	eq[7] = rho*w2*(1.f+3.f*(-vx-vy)	+4.5f*(-vx-vy)*(-vx-vy)	-vSq);
	eq[8] = rho*w2*(1.f+3.f*( vx-vy)	+4.5f*( vx-vy)*( vx-vy)	-vSq);
	
	for(int q = 0; q<Q; ++q)
	 lattice[i+q] = atau1*l[q]+atau*eq[q];
}

// Bounce back from solid surfaces
__kernel void solidBB(__global float* lattice, __global char* __read_only solid, int width)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	
	unsigned int si = getSID(coord, width);
	unsigned int i = si*Q; //Index of f0 for current node
	
	float l[Q];
	
	
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

__kernel void vWrap(__global float* lattice, int width, int height)
{
	int x = get_global_id(0);
	
	unsigned int ib = getID((int2)(x, 0), width);
	unsigned int it = getID((int2)(x, height-1), width);
	//offtop
	lattice[ib+2] = lattice[it+2];
	lattice[ib+5] = lattice[it+5];
	lattice[ib+6] = lattice[it+6];
	
	//offbottom
	lattice[it+4] = lattice[ib+4];
	lattice[it+7] = lattice[ib+7];
	lattice[it+8] = lattice[ib+8];
}

__kernel void hWrap(__global float* lattice, int width, int height)
{
	int y = get_global_id(0);
	
	unsigned int ir = getID((int2)(0, y), width);
	unsigned int il = getID((int2)(width-1, y), width);
	//offleft
	lattice[ir+1] = lattice[il+1];
	lattice[ir+5] = lattice[il+5];
	lattice[ir+8] = lattice[il+8];
	
	//offright
	lattice[il+3] = lattice[ir+3];
	lattice[il+7] = lattice[ir+7];
	lattice[il+6] = lattice[ir+6];
}

__kernel void inflow(__global float* lattice, float vx, float rho, int width)
{
	int y = get_global_id(0);
	int2 coord = (int2)(0, y);
	
	unsigned int i = getID(coord, width);
	
	float l[Q];
	
	int w0 = 4.f/9.f;
	int w1 = 1.f/9.f;
	int w2 = 1.f/36.f;
	
	float vx_term = 1.f+3.f*vx+3.f*vx*vx;
	l[1] = rho*w1*vx_term;
	l[5] = rho*w2*vx_term;
	l[8] = l[5];
	
	lattice[i+1] = l[1];
	lattice[i+5] = l[5];
	lattice[i+8] = l[8];
	
}

//Draw velocities
__kernel void render(__global float* __read_only vMags,
 __global char* __read_only solid, int width, int height,
 __global __write_only image2d_t texture)
{
	int x = get_global_id(0), y = get_global_id(1);
	int2 coord = (int2)(x, y);
	int2 scoord = (int2)(x, height-1-y);
	unsigned int si = getSID(coord, width);
	
	float minc=0.f;
	float maxc=0.1f;
	
	float frac=(vMags[si]-minc)/(maxc-minc);
	float r, g, b, a = 1.f;
	
	r = frac*0.5f;
	g = frac*1.5f;
	b = 1.f;
	
	if(solid[si])
	 r = g = b = 0.f;
	
	float4 colour = (float4)(r, g, b, a);
	write_imagef(texture, scoord, colour);
}
