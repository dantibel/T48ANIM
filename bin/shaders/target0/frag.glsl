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

layout(binding = 5) uniform sampler2D InColorTrans;
                                           
void main( void )
{  
  ivec2 screen = ivec2(gl_FragCoord);
  
  if (texelFetch(InColorTrans, screen, 0).w == 0)
    OutColor = vec4(texelFetch(InColorTrans, screen, 0).rgb, 1);
  //OutColor = vec4(1);
  //if (screen.x < 400 && screen.y < 300)
    //OutColor = vec4(texelFetch(InKsPh, screen, 0).rgb, 1);
}