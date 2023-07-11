/* snow vertex shader */
#version 330

layout(location = 0) in vec3 InPosition;

flat out float VId;

void main( void )
{
  VId = gl_VertexID;
  gl_Position = vec4(InPosition, 1);
}