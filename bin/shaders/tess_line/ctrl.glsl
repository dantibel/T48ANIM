/* */
#version 450
 
layout(vertices = 4) out;
 
uniform int AddonInt0;
uniform int AddonInt1;
 
void main( void )
{
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  gl_TessLevelOuter[0] = 1;
  gl_TessLevelOuter[1] = 64;
}