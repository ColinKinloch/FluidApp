// OpenCL LBM
//
// Run order:							6 2 5
//  1 stream							 \|/ 
//  2 boundry contitions	3-0-1
//  3 collide							 /|\ 
//  4 render							7 4 8
// Colin Kinloch - 1026970

#define D 2
#define Q 9

uint getSID(int2 coord, int width)
{
	return coord.y*width+coord.x;
}

uint getID(int2 coord, int width)
{
	return Q*getSID(coord, width);
}

__kernel void stream(__global __read_only float* oldLattice, __global __write_only float* newLattice, int width, int height, float dt)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	
	unsigned int i = getID(coord, width); //Index of f0 for current node
	
	float l[Q]; //fi of current node
	
	//int q = 0;
	
	//Copy to local
	//for(int q = 0; q<Q; ++q)
	// l[q] = oldLattice[i+q];
	
	int xl = x-1;
	int yl = y-1;
	int xh = x+1;
	int yh = y+1;
	if(x==0) xl=0;
	if(y==0) yl=0;
	if(x==(width-1)) xh=width-1;
	if(y==(height-1)) yh=height-1;
	newLattice[i] = oldLattice[i];
	newLattice[i+1] = oldLattice[getID((int2)(xl, y), width)+1];
	newLattice[i+2] = oldLattice[getID((int2)(x, yl), width)+2];
	newLattice[i+3] = oldLattice[getID((int2)(xh, y), width)+3];
	newLattice[i+4] = oldLattice[getID((int2)(x, yh), width)+4];
	newLattice[i+5] = oldLattice[getID((int2)(xl, yl), width)+5];
	newLattice[i+6] = oldLattice[getID((int2)(xh, yl), width)+6];
	newLattice[i+7] = oldLattice[getID((int2)(xh, yh), width)+7];
	newLattice[i+8] = oldLattice[getID((int2)(xl, yh), width)+8];
	
	//Copy to global
	//for(int q = 0; q<Q; ++q)
	// newLattice[i+q] = l[q];
}

__kernel void collide(__global float* lattice, __global float* vMags, int width, float tau)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	
	unsigned int si = getSID(coord, width);
	unsigned int i = si*Q; //Index of f0 for current node
	
	float w0 = 4.f/9.f;
	float w1 = 1.f/9.f;
	float w2 = 1.f/36.f;
	
	float atau = 1.f/tau;
	float atau1 = 1.f - atau;
	
	float l[Q];
	float ro, rovx, rovy;
	float vx, vy;
	float v_sq_term;
	
	for(int q=0; q<Q; ++q)
	 ro += l[q] = lattice[i+q];
	
	rovx = l[1] - l[3] + l[5] - l[6] - l[7] + l[8];
	rovy = l[2] - l[4] + l[5] + l[6] - l[7] - l[8];
	
	vx = rovx/ro;
	vy = rovy/ro;
	
	vMags[si] = sqrt(vx*vx+vy*vy);
	
	v_sq_term = 1.5f*(vx*vx + vy*vy);
	
	
	//Equilibrium
	float eq[Q];
	eq[0] = ro*w0*(1.f																			-v_sq_term);
	eq[1] = ro*w1*(1.f+3.f*vx				+4.5f*vx*vx							-v_sq_term);
	eq[2] = ro*w1*(1.f+3.f*vy				+4.5f*vy*vy							-v_sq_term);
	eq[3] = ro*w1*(1.f-3.f*vx				+4.5f*vx*vx							-v_sq_term);
	eq[4] = ro*w1*(1.f-3.f*vy				+4.5f*vy*vy							-v_sq_term);
	eq[5] = ro*w2*(1.f+3.f*( vx+vy)	+4.5f*( vx+vy)*( vx+vy)	-v_sq_term);
	eq[6] = ro*w2*(1.f+3.f*(-vx+vy)	+4.5f*(-vx+vy)*(-vx+vy)	-v_sq_term);
	eq[7] = ro*w2*(1.f+3.f*(-vx-vy)	+4.5f*(-vx-vy)*(-vx-vy)	-v_sq_term);
	eq[8] = ro*w2*(1.f+3.f*( vx-vy)	+4.5f*( vx-vy)*( vx-vy)	-v_sq_term);
	
	for(int q = 0; q<Q; ++q)
	 lattice[i+q] = atau1*l[q]+atau*eq[q];
}

//Bounce back boundry codition
__kernel void solidBC(__global float* lattice, __global char* __read_only solid, int width)
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

__kernel void wrap(__global float* lattice, int width, int height)
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

//Draw velocities
__kernel void render(__global float* __read_only vMags, __global char* __read_only solid, int width, int height,
 __global __write_only image2d_t texture, __global float* __read_only cMap)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	int2 scoord = (int2)(x, height-1-y);
	unsigned int si = getSID(coord, width); //Index of f0 for current node
	
	float minvar=0.f;
	float maxvar=0.2f;
	
	float pv = vMags[si];
	float frac=(pv-minvar)/(maxvar-minvar);
	int ncol= 236;
	int c=ncol*frac*4;
	float r = cMap[c+1], g = cMap[c+2], b = cMap[c+3], a = cMap[c];
	int s = 0;
	if(solid[si])
	 r = g = b = 0;
	
	float4 colour = (float4)(r, g, b, a);
	write_imagef(texture, scoord, colour);
}
