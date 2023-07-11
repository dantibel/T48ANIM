/* snow geometry shader */
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

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