/* PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Render data implementation.
 * PROGRAMMER: VG4
 * DATE: 17.01.2022
 */
#include "anim/rnd/rnd.h"

HWND DB6_hRndWnd;                 /* Work window handle */
HDC DB6_hRndDC;                   /* Work window memory device context  */
INT DB6_RndFrameW, DB6_RndFrameH; /* Work window size */

DBL
  DB6_RndProjSize = 0.1,     /* Project plane fit square */
  DB6_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  DB6_RndProjFarClip = 1000; /* Distance to project far clip plane (far) */

VEC
  DB6_RndCamLoc,   /* Camera location */
  DB6_RndCamAt,    /* Camera's look location */
  DB6_RndCamDir,   /* Camera's look direction */
  DB6_RndCamUp,    /* Camera's up vector */
  DB6_RndCamRight; /* Camera's right vector */

MATR
  DB6_RndMatrView, /* View coordinate system matrix */
  DB6_RndMatrProj, /* Projection coordinate system matrix */
  DB6_RndMatrVP;   /* Stored (View * Proj) matrix */

HGLRC DB6_hRndGLRC; /* OpenGL rendering context */

INT DB6_RndUBOCamera; /* UBO stock camera store number */

/* END 'rnd.c' FILE */
