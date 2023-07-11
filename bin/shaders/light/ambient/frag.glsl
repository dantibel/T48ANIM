/**/
#version 420
 
layout(location = 0) out vec4 OutColor;
 
/*
 *  G-Buffers
 * 0 – Pos.xyz   | 
 * 1 – N.xyz     | IsShade.w
 * 2 – Ka.xyz    |
 * 3 – Kd.xyz    |
 * 4 – Ks.xyz    | Ph.w
 * 5 – Color.xyz | Trans.w
 */
 
layout(binding = 1) uniform sampler2D OutNIsShade;
layout(binding = 2) uniform sampler2D OutKa;

void main( void )
{
  ivec2 screen = ivec2(gl_FragCoord.xy);
 
  vec4 NIsShade = texelFetch(OutNIsShade, screen, 0);
  vec4 Ka = texelFetch(OutKa, screen, 0);
  
  if (NIsShade.w == 0)
    discard;

  OutColor = vec4(Ka.xyz, 0);
}