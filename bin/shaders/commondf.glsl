/* commondf */
#version 450
  
/*
 *  G-Buffers
 * 0 � Pos.xyz   | 
 * 1 � N.xyz     | IsShade.w
 * 2 � Ka.xyz    |
 * 3 � Kd.xyz    |
 * 4 � Ks.xyz    | Ph.w
 * 5 � Color.xyz | Trans.w
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
