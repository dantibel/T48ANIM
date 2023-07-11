/* snow geometry shader */
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform mat4 MatrWInv;
uniform float Time;

uniform vec3 CamRight, CamUp;

in vec4 GeomColor[];
flat in float VId[];

out vec4 DrawColor;
out vec2 DrawTexCoord;
flat out float DrawId;

void main( void )
{
  float s = 2 + 0.2 * sin(Time * 3 + VId[0] * VId[0]);
  vec4 p = gl_in[0].gl_Position;
  vec3
    r = s * CamRight,
    u = s * CamUp;
	
  DrawId = VId[0];
  gl_Position = MatrWVP * vec4(p.xyz - r + u, 1);
  DrawTexCoord = vec2(0, 1);
  EmitVertex();

  gl_Position = MatrWVP * vec4(p.xyz - r - u, 1);
  DrawTexCoord = vec2(0, 0);
  EmitVertex();
  
  gl_Position = MatrWVP * vec4(p.xyz + r + u, 1);
  DrawTexCoord = vec2(1, 1);
  EmitVertex();
  
  gl_Position = MatrWVP * vec4(p.xyz + r - u, 1);
  DrawTexCoord = vec2(1, 0);
  EmitVertex();
  
  EndPrimitive();
}