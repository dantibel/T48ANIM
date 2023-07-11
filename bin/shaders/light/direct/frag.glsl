/* */
#version 430    
 
/*
 *  G-Buffers
 * 0 – Pos.xyz   | 
 * 1 – N.xyz     | IsShade.w
 * 2 – Ka.xyz    |
 * 3 – Kd.xyz    |
 * 4 – Ks.xyz    | Ph.w
 * 5 – Color.xyz | Trans.w
 */
 
layout(location = 0) out vec4 OutColor;
 
layout(binding = 0) uniform sampler2D InPos;
layout(binding = 1) uniform sampler2D InNIsShade;
layout(binding = 2) uniform sampler2D InKa;
layout(binding = 3) uniform sampler2D InKd;
layout(binding = 4) uniform sampler2D InKsPh;
layout(binding = 5) uniform sampler2D InColorTrans;

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
 
uniform vec3 LightColor, LightDir;

vec3 Shade( vec3 P, vec3 N, vec3 Kd, vec3 Ks, float Ph )
{
  vec3 L = normalize(LightDir), V = normalize(P - CamLoc), color = vec3(0);

  N = faceforward(N, V, N);
   
  // Diffuse
  color += max(0.1, dot(N, L)) * Kd * LightColor;

  // Specular                            
  vec3 R = reflect(V, N);
  color += pow(max(0.1, dot(R, L)), Ph) * Ks * LightColor;
 
  return color;
}

void main( void )
{
  ivec2 screen = ivec2(gl_FragCoord);
  vec4
    Pos = texelFetch(InPos, screen, 0),
    NIsShade = normalize(texelFetch(InNIsShade, screen, 0)),
    Ka = texelFetch(InKa, screen, 0),
    Kd = texelFetch(InKd, screen, 0),
    KsPh = texelFetch(InKsPh, screen, 0),
    ColorTrans = texelFetch(InColorTrans, screen, 0);

  if (NIsShade.w == 0)
    discard;
  vec3 sh = Shade(Pos.xyz, NIsShade.xyz, Kd.xyz, KsPh.xyz, KsPh.w).rgb;
  //OutColor = vec4(ColorTrans.rgb, 1);
  OutColor = vec4(sh, 1);
}