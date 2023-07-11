/* */
#version 430
 
layout(isolines) in;
           
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
 
void main( void )
{                 
  float t = gl_TessCoord.x, alpha = 1.0 / 6;
  vec4
    R0 = gl_in[0].gl_Position,
    R1 = gl_in[1].gl_Position,
    R2 = gl_in[2].gl_Position,
    R3 = gl_in[3].gl_Position,
    P0 = R1,
    P1 = R1 + (R2 - R0) * alpha,
    P2 = R2 + (R1 - R3) * alpha,
    P3 = R2;

  gl_Position = MatrWVP * vec4(P0 * (1 - t) * (1 - t) * (1 - t) +
                               P1 * 3 * (1 - t) * (1 - t) * t +
                               P2 * 3 * (1 - t) * t * t +
                               P3 * t * t * t);
}