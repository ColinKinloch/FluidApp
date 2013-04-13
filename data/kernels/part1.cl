float addem(float a, float b)
{
	return a + b;
}

__kernel void part1(__global float* a, __global float* b, __global float* c)
{
	unsigned int i = get_global_id(0);
	
	c[i] = addem(a[i], b[i]);
}
