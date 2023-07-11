/* */
#version 430
 
layout(quads) in;
 
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

out vec3 Normal;
 
vec3 Sphere( vec2 uv )
{
  float
    pi = acos(-1),
    t = uv.y * pi,
    p = uv.x * 2 * pi;
 
  return vec3(sin(t) * sin(p), cos(t), sin(t) * cos(p));
}
 
void main( void )
{                 
  float R = 0.05;
 
  gl_Position = MatrWVP * vec4((Normal = Sphere(gl_TessCoord.xy)) * R, 1);
}