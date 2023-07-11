/* */
#version 450
 
layout(location = 0) out vec4 OutPos;
layout(location = 1) out vec4 OutNIsShade;
layout(location = 2) out vec4 OutKa;
layout(location = 3) out vec4 OutKd;
layout(location = 4) out vec4 OutKsPh;
layout(location = 5) out vec4 OutColorTrans;
 
in vec3 Normal;
 
void main( void )
{
  vec3 N = normalize(Normal);
  vec3 L = normalize(vec3(1, 1, 1));
  float c = 0.1;
  //float c = max(0.1, dot(N, L));
  OutNIsShade = vec4(N, 1);
  OutKa = vec4(vec3(c), 0);
  OutKd = vec4(vec3(0.7), 0);
  OutKsPh = vec4(vec3(0.001), 0.1);
  OutColorTrans = vec4(vec3(1) * c, 1);
}