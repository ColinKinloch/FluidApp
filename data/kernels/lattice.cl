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

__kernel void stream(__global __read_only float* lattice, __global __write_only float* tmpLattice, int width, int height, float dt)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	
	unsigned int i = getID(coord, width); //Index of f0 for current node
	
	float l[Q]; //fi of current node
	
	//int q = 0;
	
	//Copy to local
	for(int q = 0; q<Q; ++q)
	 l[q] = lattice[i+q];
	
	int xl = x-1;
	int yl = y-1;
	int xh = x+1;
	int yh = y+1;
	if(x==0) xl=width-1;
	if(y==0) yl=height-1;
	if(x==width) xh=0;
	if(y==height) yh=0;
	
	l[1] = lattice[getID((int2)(xl, y), width)+1];
	l[2] = lattice[getID((int2)(x, yl), width)+2];
	l[3] = lattice[getID((int2)(xh, y), width)+3];
	l[4] = lattice[getID((int2)(x, yh), width)+4];
	l[5] = lattice[getID((int2)(xl, yl), width)+5];
	l[6] = lattice[getID((int2)(xh, yl), width)+6];
	l[7] = lattice[getID((int2)(xh, yh), width)+7];
	l[8] = lattice[getID((int2)(xl, yh), width)+8];
	
	//Copy to global
	for(int q = 0; q<Q; ++q)
	 tmpLattice[i+q] = l[q];
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
	
	float eq[Q];
	float l[Q];
	float ro, rovx, rovy;
	float vx, vy;
	for(int q=0; q<Q; ++q)
	{
		l[q] = lattice[i+q];
		ro += l[q];
	}
	rovx = l[1] - l[3] + l[5] - l[6] - l[7] + l[8];
	rovy = l[2] - l[4] + l[5] + l[6] - l[7] - l[8];
	
	vx = rovx/ro;
	vy = rovy/ro;
	
	vMags[si] = sqrt(vx*vx + vy*vy);
	
	float v_sq_term = 1.5f*(vx*vx + vy*vy);
	
	float atau = 1.f/tau;
	float atau1 = 1.f - atau;
	
	eq[0] = ro*w0*(1.f-v_sq_term);
	eq[1] = ro*w1*(1.f+3.f*vx+4.5f*vx*vx-v_sq_term);
	eq[2] = ro*w1*(1.f+3.f*vy+4.5f*vy*vy-v_sq_term);
	eq[3] = ro*w1*(1.f-3.f*vx+4.5f*vx*vx-v_sq_term);
	eq[4] = ro*w1*(1.f-3.f*vy+4.5f*vy*vy-v_sq_term);
	eq[5] = ro*w2*(1.f+3.f*(vx+vy)+4.5f*(vx+vy)*(vx+vy)-v_sq_term);
	eq[6] = ro*w2*(1.f+3.f*(-vx+vy)+4.5f*(-vx+vy)*(-vx+vy)-v_sq_term);
	eq[7] = ro*w2*(1.f+3.f*(-vx-vy)+4.5f*(-vx-vy)*(-vx-vy)-v_sq_term);
	eq[8] = ro*w2*(1.f+3.f*(vx-vy)+4.5f*(vx-vy)*(vx-vy)-v_sq_term);
	
	for(int q = 0; q<Q; ++q)
	 l[q] = atau1*l[q]+atau*eq[q];
	
	//Copy to global
	for(int q = 0; q<Q; ++q)
	 lattice[i+q] = l[q];
}

__kernel void solidBC(__global float* lattice, __global char* __read_only solid, int width)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	
	unsigned int si = getSID(coord, width);
	unsigned int i = si*Q; //Index of f0 for current node
	
	float l[Q];
	
	
	if(solid[si])
	{
	for(int q=0; q<Q; ++q)
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

__kernel void render(__global float* __read_only vMags, __global char* __read_only solid, int width, int height,
 __global __write_only image2d_t texture)
{
	int x = get_global_id(0), y = get_global_id(1); //Coordinate of current node
	int2 coord = (int2)(x, y); //Coordinatie of current node
	
	unsigned int si = getSID(coord, width); //Index of f0 for current node
	
	float minvar=0.f;
	float maxvar=0.f;
	
	float pv = vMags[si];
	float frac=(pv-minvar)/(maxvar-minvar);
	int ncol= 1;
	float c=frac*ncol;
	
	float r = 0.f, g = 0.f, b = 1.f;
	
	r = pv;
	g = 1-pv;
	b = 1-pv;
	
	if(solid[si])
	 r = g = b = 0;
	
	float4 colour = (float4)(r, g, b, 1);
	write_imagef(texture, coord, colour);
}
