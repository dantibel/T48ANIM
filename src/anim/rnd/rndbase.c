/* FILE NAME: rndbase.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Basic render function implementation.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#include "anim/anim.h"

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *      UINT Source;
 *   - error type:
 *      UINT Type;
 *   - error message id:
 *      UINT Id, 
 *   - message severity:
 *      UINT severity, 
 *   - message text length:
 *      INT Length, 
 *   - message text:
 *      CHAR *Message, 
 *   - user addon parameters pointer:
 *      VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message, const VOID *UserParam )
{
  INT len = 0;
  static CHAR Buf[10000];

  /* Ignore non-significant error/warning codes */
  if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
    return;

  len += sprintf(Buf + len, "Debug message (%i) %s\n", Id, Message);
  switch (Source)
  {
  case GL_DEBUG_SOURCE_API:
    len += sprintf(Buf + len, "Source: API\n");
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    len += sprintf(Buf + len, "Source: Window System\n");
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    len += sprintf(Buf + len, "Source: Shader Compiler\n");
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    len += sprintf(Buf + len, "Source: Third Party\n");
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    len += sprintf(Buf + len, "Source: Application");
    break;
  case GL_DEBUG_SOURCE_OTHER:
    len += sprintf(Buf + len, "Source: Other");
    break;
  }
  len += sprintf(Buf + len, "\n");

  switch (Type)
  {
  case GL_DEBUG_TYPE_ERROR:
    len += sprintf(Buf + len, "Type: Error");
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    len += sprintf(Buf + len, "Type: Deprecated Behaviour");
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    len += sprintf(Buf + len, "Type: Undefined Behaviour");
    break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    len += sprintf(Buf + len, "Type: Portability");
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    len += sprintf(Buf + len, "Type: Performance");
    break;
  case GL_DEBUG_TYPE_MARKER:
    len += sprintf(Buf + len, "Type: Marker");
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    len += sprintf(Buf + len, "Type: Push Group");
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    len += sprintf(Buf + len, "Type: Pop Group");
    break;
  case GL_DEBUG_TYPE_OTHER:
    len += sprintf(Buf + len, "Type: Other");
    break;
  }
  len += sprintf(Buf + len, "\n");

  switch (Severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    len += sprintf(Buf + len, "Severity: high");
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    len += sprintf(Buf + len, "Severity: medium");
    break;
  case GL_DEBUG_SEVERITY_LOW:
    len += sprintf(Buf + len, "Severity: low");
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    len += sprintf(Buf + len, "Severity: notification");
    break;
  }
  len += sprintf(Buf + len, "\n\n");

  OutputDebugString(Buf);
} /* End of 'glDebugOutput' function */

/* Render initialization function.
 * ARGUMENTS:
 *   - handle of window:
 *       HWND hWnd.
 * RETURNS: None.
 */
VOID DB6_RndInit( HWND hWnd )
{
  INT
    i, nums,
    PixelAttribs[] =
    {
      WGL_DRAW_TO_WINDOW_ARB, TRUE,
      WGL_SUPPORT_OPENGL_ARB, TRUE,
      WGL_DOUBLE_BUFFER_ARB, TRUE,
      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_COLOR_BITS_ARB, 32,
      WGL_DEPTH_BITS_ARB, 32,
      0
    },
    ContextAttribs[] =
    {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
      WGL_CONTEXT_MINOR_VERSION_ARB, 6,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      0
    };
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;

  DB6_hRndWnd = hWnd;
  DB6_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DB6_hRndDC, &pfd);
  DescribePixelFormat(DB6_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(DB6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  DB6_hRndGLRC = wglCreateContext(DB6_hRndDC);
  wglMakeCurrent(DB6_hRndDC, DB6_hRndGLRC);

  /* Initialize GLEW library */
  if (glewInit() != GLEW_OK /* || !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) */)
  {
    MessageBox(hWnd, "Error OpenGL initialization", "ERROR",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

    /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(DB6_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(DB6_hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DB6_hRndGLRC);

  DB6_hRndGLRC = hRC;
  wglMakeCurrent(DB6_hRndDC, DB6_hRndGLRC);

  /* Turn off V-syncronization */
  wglSwapIntervalEXT(0);

#ifdef _DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
  glDebugMessageCallback(glDebugOutput, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif /* _DEBUG */

  /* Set default render parameters */
  glClearColor(0.30, 0.47, 0.8, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Setup rendered scene parameters */
  DB6_RndFrameW = DB6_RndFrameH = 101;
  DB6_RndProjDist = DB6_RndProjSize = 0.1;
  DB6_RndProjFarClip = 1500;
  DB6_RndMatrView = DB6_RndMatrProj = MatrIdentity();
  DB6_RndCamSet(VecSet(0, 20, 10), VecSet(0, 15, 0), VecSet(0, 1, 0));
  DB6_RndProjSet();

  DB6_RndResInit();

  /* Appling UBOs */
  DB6_RndUBOCamera = DB6_RndUBOCreate("Camera", DB6_RND_UBO_BIND_CAMERA, sizeof(db6RND_UBO_MATR) / 16, NULL);
  DB6_RndUBOApply(DB6_RndUBOCamera);

  DB6_RndTargetInit();
} /* End of 'DB6_RndInit' function */

/* Render close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndClose( VOID )
{ 
  DB6_RndTargetClose();
  DB6_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DB6_hRndGLRC);
  ReleaseDC(DB6_hRndWnd, DB6_hRndDC);
} /* End of 'DB6_RndClose' function */

/* Render resizing function.
 * ARGUMENTS:
 *   - handle of window:
 *       HWND hWnd.
 * RETURNS: None.
 */
VOID DB6_RndResize( INT W, INT H )
{
  db6RND_UBO_MATR CamU;

  /* Saving size */
  DB6_RndFrameW = W;
  DB6_RndFrameH = H;

  /* Set up projection */
  DB6_RndProjSet();

  /* Set camera */
  CamU.CamLoc = Vec4SetVec3(DB6_RndCamLoc);
  CamU.CamDir = Vec4SetVec3(DB6_RndCamDir);
  CamU.CamRight = Vec4SetVec3(DB6_RndCamRight);
  CamU.CamUp = Vec4SetVec3(DB6_RndCamUp);
  CamU.FrameWHProjDistFar = Vec4Set(DB6_RndFrameW, DB6_RndFrameH, DB6_RndProjDist, DB6_RndProjFarClip);
  CamU.MatrVP = CamU.MatrWVP = DB6_RndMatrVP;
  CamU.MatrV = DB6_RndMatrView;
  CamU.MatrW = CamU.MatrWInv = MatrIdentity();
  CamU.TimeGlobalDeltaGlobal = Vec4Set(DB6_Anim.Time, DB6_Anim.GlobalTime, DB6_Anim.DeltaTime, DB6_Anim.GlobalDeltaTime);

  DB6_RndUBOUpdate(DB6_RndUBOCamera, 0, 0, &CamU);

  DB6_RndTargetResize(W, H);
} /* End of 'DB6_RndResize' function */
 
/* Render redraw frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndCopyFrame( VOID )
{
  wglSwapLayerBuffers(DB6_hRndDC, WGL_SWAP_MAIN_PLANE);
}  /* End of 'DB6_RndCopyFrame' function */

/* Start render function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndStart( VOID )
{
  static DBL ReloadTime = 0;
  db6RND_UBO_MATR CamU;
  INT modes[2];

  /* Update shaders */
  if (DB6_Anim.GlobalTime - ReloadTime > 3)
  {
    ReloadTime = DB6_Anim.GlobalTime;
    DB6_RndShdUpdate();
  }

  /* Set camera */
  CamU.CamLoc = Vec4SetVec3(DB6_RndCamLoc);
  CamU.CamDir = Vec4SetVec3(DB6_RndCamDir);
  CamU.CamRight = Vec4SetVec3(DB6_RndCamRight);
  CamU.CamUp = Vec4SetVec3(DB6_RndCamUp);
  CamU.FrameWHProjDistFar = Vec4Set(DB6_RndFrameW, DB6_RndFrameH, DB6_RndProjDist, DB6_RndProjFarClip);
  CamU.MatrVP = CamU.MatrWVP = DB6_RndMatrVP;
  CamU.MatrV = DB6_RndMatrView;
  CamU.MatrW = CamU.MatrWInv = MatrIdentity();
  CamU.TimeGlobalDeltaGlobal = Vec4Set(DB6_Anim.Time, DB6_Anim.GlobalTime, DB6_Anim.DeltaTime, DB6_Anim.GlobalDeltaTime);
  glGetIntegerv(GL_POLYGON_MODE, modes);
  CamU.Wireframe = Vec4Set(modes[0] == GL_LINE, 0, 0, 0);

  DB6_RndUBOUpdate(DB6_RndUBOCamera, 0, 0, &CamU);

  DB6_RndTargetStart();
} /* End of 'DB6_RndStart' function */

/* Render initialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndEnd( VOID )
{
  glFinish();
  DB6_RndTargetEnd();
} /* End of 'DB6_RndEnd' function */

/* Set render projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = DB6_RndProjSize;

  /* Correct aspect ratio */
  if (DB6_RndFrameW > DB6_RndFrameH)
    rx *= (DBL)DB6_RndFrameW / DB6_RndFrameH;
  else
    ry *= (DBL)DB6_RndFrameH / DB6_RndFrameW;

  /* Recalculating projection matrix */
  DB6_RndMatrProj = MatrOrtho(-rx / 2, rx / 2, -ry / 2, ry / 2, DB6_RndProjDist, DB6_RndProjFarClip);
  DB6_RndMatrVP = MatrMulMatr(DB6_RndMatrView, DB6_RndMatrProj);
} /* End of 'DB6_RndProjSet' function */

/* Set camera position function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  /* Set world transformation matrix */
  DB6_RndMatrView = MatrView(Loc, At, Up);
  DB6_RndMatrVP = MatrMulMatr(DB6_RndMatrView, DB6_RndMatrProj);

  /* Store camera parameters */
  DB6_RndCamLoc = Loc;
  DB6_RndCamAt = At;
  DB6_RndCamUp = Up;
  DB6_RndCamDir = VecSet(-DB6_RndMatrView.A[0][2], -DB6_RndMatrView.A[1][2], -DB6_RndMatrView.A[2][2]);
  DB6_RndCamRight = VecSet(DB6_RndMatrView.A[0][0], DB6_RndMatrView.A[1][0], DB6_RndMatrView.A[2][0]);
} /* End of 'DB6_RndCamSet' function */

/* END OF 'rndbase.c' FILE */
