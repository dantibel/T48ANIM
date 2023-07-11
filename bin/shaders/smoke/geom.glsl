/* smoke geometry shader */
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

vec3 Rot( float x, float y, float angle )
{
  angle *= acos(-1) / 180;
  float s = sin(angle), c = cos(angle);

  return CamRight * (x * c - y * s) + CamUp * (x * s + y * c);                                                 
}

void main( void )
{
  int Id = int(VId[0]); 
  float full_time = 30, age = mod(Id + Time / 3.0, full_time) / full_time,
        h = 3 * age, s, a = /* pow(-1, Id) */ * mix(0, 180, 180 * age / full_time);
  vec3 p = vec3(5, 13 + 5 * h, 5);
  vec3
    r = vec3(CamRight.xyz), u = vec3(CamUp.xyz);

  vec4 c_g = vec4(0.1, 0.1, 0.1, 1), c_r = vec4(1, 0, 0, 1), c_y = vec4(1, 1, 0, 1),
       c_o = vec4(1, 0.5, 0, 1);
	
  s = mix(1.0, 5.0, age);
  r *= s, u *= s;

  DrawId = VId[0];
  if (age < 1)
    DrawColor = mix(c_r, c_g, age);
  if (age < 0.3)
    DrawColor = mix(c_o, c_r, (age - 0.1) / 0.2);
  if (age < 0.1)
    DrawColor = mix(c_y, c_o, age / 0.1);
  DrawColor.a = age < 0.5 ? 1 : mix(1.0, 0.0, age / full_time);

  gl_Position = MatrWVP * vec4(p.xyz + Rot(-1, 1, a) * s, 1);
  DrawTexCoord = vec2(0, 1);
  EmitVertex();
  gl_Position = MatrWVP * vec4(p.xyz + Rot(-1, -1, a) * s, 1);
  DrawTexCoord = vec2(0, 0);
  EmitVertex();
  gl_Position = MatrWVP * vec4(p.xyz + Rot(1, 1, a) * s, 1);
  DrawTexCoord = vec2(1, 1);
  EmitVertex();
  gl_Position = MatrWVP * vec4(p.xyz + Rot(1, -1, a) * s, 1);
  DrawTexCoord = vec2(1, 0);
  EmitVertex();
  EndPrimitive();
}