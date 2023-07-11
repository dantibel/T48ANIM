/* snow vertex shader */
#version 330

layout(location = 0) in vec3 InPosition;

#define N 2
void main( void )
{
  float 
    n_pnt = gl_VertexID & 4,
    n_blk = gl_VertxeID / 4,
    row = n_blk / N,
    col = n_blk % N;
  mat4 v = mat4(vec3(0, 0, 0), 
                vec3(1, 0, 0),
                vec3(0, 0, -1),
                vec3(1, 0, -1));
  gl_Position = vec4(v[n_pnt].xyz * vec3(col, 0, row), 1);
}