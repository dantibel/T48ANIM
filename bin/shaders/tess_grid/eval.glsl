/* */
#version 430
 
layout(quads) in;
 
uniform mat4 MatrWVP;
 
out vec3 Normal;
 
vec3 SphereTrans( vec2 uv )
{
  float
    pi = acos(-1),
    t = uv.y * pi,
    p = uv.x * 2 * pi;
 
  return vec3(sin(t) * sin(p), cos(t), sin(t) * cos(p));
}
 
void main( void )
{ 
  float C[] = {1.0, 3.0, 3.0, 1.0};
  vec3 Grid[4][4], P[4][4]; 
  /*
  {
    {{-3, 3, -2}, {-1, -1, -2}, {1, 1, -2}, {3, -3, -2}},
    {{-3, 3, 0}, {-1, -1, 0}, {1, 1, 0}, {{3, -3, 0}},
    {{-3, 3, 2}, {-1, -1, 2}, {1, 1, 2}, {{3, -3, 2}},
    {{-3, 3, 4}, {-1, -1, 4}, {1, 1, 4}, {{3, -3, 4}}
  };
  */

  for (int i = 0; i < 4; i++)
  {
    float
      v = i / 3,
      vs[] = {1, v, v * v, v * v * v},
      vs1[] = {1, v - 1, (v - 1) * (v - 1), (v - 1) * (v - 1) * (v - 1)};

    for (int j = 0; j < 4; j++)
    {
      float
        u = i / 3,
        us[] = {1, u, u * u, u * u * u},
        us1[] = {1, u - 1, (u - 1) * (u - 1), (u - 1) * (u - 1) * (u - 1)};

      Grid[i][j] = vec3(0);
      P[i][j] = gl_in[i * 4 + j].gl_Position.xyz;
      for (int ii = 0; ii < 4; ii++)
        for (int jj = 0; jj < 4; jj++)
        {                                             
          Grid[i][j] += P[ii][jj] * C[i] * C[j] * us1[j] * us[3 - j] * vs1[i] * vs[3 - i];
          gl_Position = MatrWVP * vec4(Grid[i][j], 1);
        }
    }
  }
}