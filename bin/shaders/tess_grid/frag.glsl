/* */
#version 450
 
layout(location = 0) out vec4 OutColor;
 
in vec3 Normal;
 
void main( void )
{
  vec3 N = normalize(Normal);
  vec3 L = normalize(vec3(1, 1, 1));
  float c = max(0.1, dot(N, L));
 
  OutColor = vec4(vec3(1, 1, 1) * c, 1);
}