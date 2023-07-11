/* */
#version 450

layout(vertices = 4) out;

uniform int AddonInt0;

void main( void )
{
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  gl_TessLevelOuter[0] = AddonInt0;
  gl_TessLevelOuter[1] = AddonInt0;
  gl_TessLevelOuter[2] = AddonInt0;
  gl_TessLevelOuter[3] = AddonInt0;
  gl_TessLevelInner[0] = AddonInt0;
  gl_TessLevelInner[1] = AddonInt0;
}