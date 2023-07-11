/**/
#version 420 
 
/*
 *  G-Buffers
 * 0 – Pos.xyz   | 
 * 1 – N.xyz     | IsShade.w
 * 2 – Ka.xyz    |
 * 3 – Kd.xyz    |
 * 4 – Ks.xyz    | Ph.w
 * 5 – Color.xyz | Trans.w
 */
 
layout(location = 0) out vec4 OutPos;
layout(location = 1) out vec4 OutNIsShade;
layout(location = 2) out vec4 OutKa;
layout(location = 3) out vec4 OutKd;
layout(location = 4) out vec4 OutKsPh;
layout(location = 5) out vec4 OutColorTrans;

layout(std140, binding = 0) uniform Matrices
{
  mat4 MatrWVP;  /* World * View * Proj matrix */
  mat4 MatrVP;   /* View * Proj matrix */
  mat4 MatrV;    /* View matrix */
  mat4 MatrW;    /* World matrix */
  mat4 MatrWInv; /* World inverse transpose matrix */
  vec4 CamLoc;   /* Camera location */
  vec4 CamRight; /* Camera right direction */
  vec4 CamUp;    /* Camera up direction */
  vec4 CamDir;   /* Camera forward direction */
  vec4 FrameWHProjDistFar;    /* Frame size and project distance and far clip */
  vec4 TimeGlobalDeltaGlobal; /* Time/global time, delta time/global delta time */
  vec4 Wireframe;             /* Wireframe flag */
};
#define Time TimeGlobalDeltaGlobal.x
#define CamLoc CamLoc.xyz
#define CamRight CamRight.xyz
#define CamUp CamUp.xyz
#define CamDir CamDir.xyz
#define FrameW FrameWHProjDistFar.x
#define FrameH FrameWHProjDistFar.y
#define ProjDist FrameWHProjDistFar.z 

// Material
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform float Trans;
uniform bool IsTexture0;
 
in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex;
  
void main( void )
{
  // Fill G-Buffers
  OutPos = vec4(DrawPos, 1);
  OutNIsShade = vec4(normalize(DrawNormal), 1);
  OutKa = vec4(Ka.xyz, 0);
  vec3 kdc = Kd;
  if (IsTexture0)
  {
    vec4 tc = texture(Tex, DrawTexCoord);
    kdc *= tc.rgb;
    OutColorTrans = vec4(kdc, 1);
  }
  else
    OutColorTrans = vec4(DrawColor.rgb, 1);	
  OutKd = vec4(kdc, 0);
  OutKsPh = vec4(Ks, Ph);
  //vec4(Shade(DrawPos, normalize(DrawNormal)), Trans);
  //vec4(DrawColor.rgb, Trans);
}
           
