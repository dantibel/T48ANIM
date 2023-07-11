/* */
#version 450
 
/*
 *  G-Buffers
 * 0 – Pos.xyz   | 
 * 1 – N.xyz     | IsShade.w
 * 2 – Ka.xyz    |
 * 3 – Kd.xyz    |
 * 4 – Ks.xyz    | Ph.w
 * 5 – Color.xyz | Trans.w
 */
 
layout(location = 0) out vec4 OutPos;
layout(location = 1) out vec4 OutNIsShade;
layout(location = 2) out vec4 OutKa;
layout(location = 3) out vec4 OutKd;
layout(location = 4) out vec4 OutKsPh;
layout(location = 5) out vec4 OutColorTrans;
 
void main( void )
{
  OutNIsShade = vec4(vec3(0), 1);
  OutKa = vec4(vec3(1), 0);
  OutKd = vec4(vec3(1), 0);
  OutKsPh = vec4(vec3(1), 0);
  OutColorTrans = vec4(1);
}