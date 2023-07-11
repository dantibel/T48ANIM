/* FILE NAME: rndtrgt.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementation module.
 *          Render targets implementation.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#include "rnd.h"

/* Render targets data */
static UINT DB6_RndTargetFBO;

/* Target shaders */
static INT
  DB6_RndTargetShd,
  DB6_RndTargetShdLightAmbient,
  DB6_RndTargetShdLightDirect;
  DB6_RndTargetShdLightPoint;

/* Target textures */
#define DB6_RND_TARGETS 6
static INT DB6_RndTargetTexId[DB6_RND_TARGETS];

/* Target buffers */
static INT DB6_RndTargetBuf, DB6_RndTargetRndBuf;

/* Light parameters */
static VEC
  LightColor = {1, 1, 1},
  LightDir = {1, -1, 0};

/* Render targets initialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetInit( VOID )
{
  /* loading shaders */
  DB6_RndTargetShd = DB6_RndShdAdd("target0");
  DB6_RndTargetShdLightAmbient = DB6_RndShdAdd("light/ambient");
  DB6_RndTargetShdLightDirect = DB6_RndShdAdd("light/direct");
  DB6_RndTargetShdLightPoint = DB6_RndShdAdd("light/point");

  /* Create target */
  DB6_RndTargetCreate(DB6_RndFrameW, DB6_RndFrameH);
} /* End of 'DB6_RndTargetInit' fuction */

/* Render targets deinitialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetClose( VOID )
{
  DB6_RndTargetFree();
} /* End of 'DB6_RndTargetClose' fuction */

/* create render targets function.
 * ARGUMENTS:
 *   - frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID DB6_RndTargetCreate( INT W, INT H )
{
  INT i, DrawBuffer[DB6_RND_TARGETS], status = 0;

  /* Create frame buffer */
  glGenFramebuffers(1, &DB6_RndTargetFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, DB6_RndTargetFBO);

  /* Create attachment textures */
  glGenTextures(DB6_RND_TARGETS, DB6_RndTargetTexId);
  for (i = 0; i < DB6_RND_TARGETS; i++)
  {
    DrawBuffer[i] = GL_COLOR_ATTACHMENT0 + i;
    glBindTexture(GL_TEXTURE_2D, DB6_RndTargetTexId[i]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, W, H);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, DB6_RndTargetTexId[i], 0);
  }

  /* Draw textures */
  glDrawBuffers(DB6_RND_TARGETS, DrawBuffer);

  /* Create render buffers */
  glGenRenderbuffers(1, &DB6_RndTargetRndBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, DB6_RndTargetRndBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, W, H);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DB6_RndTargetRndBuf);

  /* Check success of creating FBO */
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    OutputDebugString("Error create FBO\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
} /* End of 'DB6_RndTargetCreate' fuction */

/* Delete render targets function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetFree( VOID )
{
  INT i;

  glBindFramebuffer(GL_FRAMEBUFFER, DB6_RndTargetFBO);

  /* Delete textures */
  for (i = 0; i < DB6_RND_TARGETS; i++)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
  glDeleteTextures(DB6_RND_TARGETS, DB6_RndTargetTexId);

  /* Delete render buffer */
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
  glDeleteRenderbuffers(1, &DB6_RndTargetRndBuf);

  /* Delete frame buffer */
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &DB6_RndTargetFBO);
} /* End of 'DB6_RndTargetFree' fuction */

/* Start rendering targets function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetStart( VOID )
{
  INT i;
  FLT col[4] = {0.25, 0.25, 0.25, 1}, depth = 1;

  glBindFramebuffer(GL_FRAMEBUFFER, DB6_RndTargetFBO);
  for (i = 0; i < DB6_RND_TARGETS; i++)
    glClearBufferfv(GL_COLOR, i, col);
  glClearBufferfv(GL_DEPTH, 0, &depth);

  glBlendFunc(GL_ONE, GL_ZERO);
} /* End of 'DB6_RndTargetStart' fuction */

/* End rendering targets function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetEnd( VOID )
{
  INT i, modes[2], prg = 0, loc = 0;
  FLT col[4] = {0.25, 0.25, 0.25, 1}, depth = 1;
  VEC LightColor = {1, 1, 1}, LightDir = {0, 1, 1};

  glGetIntegerv(GL_POLYGON_MODE, modes);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearBufferfv(GL_COLOR, 0, col);
  glClearBufferfv(GL_DEPTH, 0, &depth);

  for (i = 0; i < DB6_RND_TARGETS; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, DB6_RndTargetTexId[i]);
  }

  glBlendFunc(GL_ONE, GL_ONE);
  glDisable(GL_DEPTH_TEST);

  /* Cast ambient light component */
  glUseProgram(DB6_RndShaders[DB6_RndTargetShdLightAmbient].ProgId);
  glDrawArrays(GL_POINTS, 0, 1);

  /* Cast directional light component */
  glUseProgram(prg = DB6_RndShaders[DB6_RndTargetShdLightDirect].ProgId);
  if ((loc = glGetUniformLocation(prg, "LightColor")) != -1)
    glUniform3fv(loc, 1, &LightColor.X);
  if ((loc = glGetUniformLocation(prg, "LightDir")) != -1)
    glUniform3fv(loc, 1, &LightDir.X);
  glDrawArrays(GL_POINTS, 0, 1);

  /* Cast zero target */
  glUseProgram(DB6_RndShaders[DB6_RndTargetShd].ProgId);
  glDrawArrays(GL_POINTS, 0, 1);
  glUseProgram(0);

  glPolygonMode(GL_BACK, modes[1]);
  glPolygonMode(GL_FRONT, modes[0]);
  glEnable(GL_DEPTH_TEST);
  glFinish();
} /* End of 'DB6_RndTargetFree' fuction */

/* Resize render target frame buffer function.
 * ARGUMENTS:
 *   - frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID DB6_RndTargetResize( INT W, INT H )
{
  DB6_RndTargetFree();
  DB6_RndTargetCreate(W, H);
  glViewport(0, 0, W, H);
} /* End of 'DB6_RndTargetResize' fuction */

/* END OF 'rndtrgt.c' FILE */
