/* sky geometry shader */
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out; 

out vec2 DrawTexCoord;

void main( void )
{                          
  gl_Position = vec4(-1, 1, 0, 1);
  EmitVertex();

  gl_Position = vec4(-1, -1, 0, 1);
  EmitVertex();
    
  gl_Position = vec4(1, 1, 0, 1);
  EmitVertex();

  gl_Position = vec4(1, -1, 0, 1);
  EmitVertex();

  EndPrimitive();
}