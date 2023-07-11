/* smoke fragment shader */
#version 420

layout(location = 0) out vec4 OutColor;

// Camera
uniform vec3 CamLoc;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;
flat in float DrawId;

uniform float Time;

layout(binding = 0) uniform sampler2D Tex;

uniform bool IsTexture0;

// Wireframe flag
uniform bool IsWireframe;

void main( void )
{
  if (IsWireframe)
  {
    OutColor = vec4(1);
    return;
  }
  vec4 tc = texture(Tex, DrawTexCoord);
  //if (tc.a < 0.8)
    //discard;
  OutColor = vec4(tc.rgb * DrawColor.rgb, tc.a);
}
