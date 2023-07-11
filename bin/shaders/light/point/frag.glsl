/* */
#version 430    
 
/*
 *  G-Buffers
 * 0 – Pos.xyz   | 
 * 1 – N.xyz     | IsShade.w
 * 2 – Ka.xyz    |
 * 3 – Kd.xyz    |
 * 4 – Ks.xyz    | Ph.w
 * 5 – Color.xyz | Trans.w
 */
 
layout(location = 0) out vec4 OutColor;
 
layout(binding = 0) uniform sampler2D InPos;
layout(binding = 1) uniform sampler2D InNIsShade;
layout(binding = 2) uniform sampler2D InKa;
layout(binding = 3) uniform sampler2D InKd;
layout(binding = 4) uniform sampler2D InKsPh;
layout(binding = 5) uniform sampler2D InColorTrans;

uniform bool IsWireframe;

uniform vec3 Ka;
 
void main( void )
{
  if (IsWireframe)
    OutColor = vec4(1);
  OutColor = vec4(Ka.rgb, 1);
}