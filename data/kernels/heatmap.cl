//Colour map
static float4 colourMap(float v)
{
  float r,g,b,a = 1.f;

  //is it opposite to what is expected
  //v = 1-v;

  //it is greater than 1 and lower than 0
  if(v>1)
    v=1.f;
  else if(v<0)
    v=0.f;


  //
  // 1|   ╱╲     ╱╲     ╱╲
  //  │  ╱  ╲   ╱  ╲   ╱  ╲
  //  │ ╱    ╲ ╱    ╲ ╱    ╲
  //  │╱      ╳      ╳      ╲
  //  │      ╱ ╲    ╱ ╲
  //  │     ╱   ╲  ╱   ╲
  //  │    ╱     ╲╱     ╲
  // 0┼──────────────────────┐
  //  0                      1
  b = 1-3*fabs(v-(1.f/6));
  g = 1-3*fabs(v-(3.f/6));
  r = 1-3*fabs(v-(5.f/6));

  //printf("hi");

  return (float4)(r,g,b,a);
}
static float4 streamlines(float v)
{
  float r,g,b,a = 0.f;
  if(v==1)
  {
    r = g = b = 1.f;
  }
  return (float4)(r,g,b,a);
}
static float4 oldMap(float v)
{
  float r,g,b,a = 1.f;

  r = v*0.25f;
  g = v*0.5f;
  b = 1.f;

  return (float4)(r,g,b,a);
}
