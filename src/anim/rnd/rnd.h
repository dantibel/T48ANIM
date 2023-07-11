/* FILE NAME: rnd.h
 * PURPOSE: 3D animation project.
 *          Rendering system declaration module.
 *          Rendering syste declaration.
 * PROGRAMMER: DB6
 * DATE: 28.02.2022
 */
#ifndef __rnd_h_
#define __rnd_h_

#include "res/rndres.h"

extern HWND DB6_hRndWnd;                 /* Work window handle */
extern HDC DB6_hRndDC;                   /* Work window memory device context  */
extern INT DB6_RndFrameW, DB6_RndFrameH; /* Work window size */

extern DBL
  DB6_RndProjSize,    /* Project plane fit square */
  DB6_RndProjDist,    /* Distance to project plane from viewer (near) */
  DB6_RndProjFarClip; /* Distance to project far clip plane (far) */

extern VEC
  DB6_RndCamLoc,   /* Camera location */
  DB6_RndCamAt,    /* Camera's look location */
  DB6_RndCamDir,   /* Camera's look direction */
  DB6_RndCamUp,    /* Camera's up vector */
  DB6_RndCamRight; /* Cameras's roght vector */

extern MATR
  DB6_RndMatrView, /* View coordinate system matrix */
  DB6_RndMatrProj, /* Projection coordinate system matrix */
  DB6_RndMatrVP;   /* Stored (View * Proj) matrix */

extern HGLRC DB6_hRndGLRC; /* OpenGL rendering context */

extern INT DB6_RndUBOCamera; /* UBO stock camera store number */

 
/* Bind points */
#define DB6_RND_UBO_BIND_CAMERA 0
 
/* Camera UBO store type */
typedef struct tagdb6RND_UBO_MATR
{
  MATR MatrWVP;  /* World * View * Proj matrix */
  MATR MatrVP;   /* View * Proj matrix */
  MATR MatrV;    /* View matrix */
  MATR MatrW;    /* World matrix */
  MATR MatrWInv; /* World inverse transpose matrix */
  VEC4 CamLoc;   /* Camera location */
  VEC4 CamRight; /* Camera right direction */
  VEC4 CamUp;    /* Camera up direction */
  VEC4 CamDir;   /* Camera forward direction */
  VEC4 FrameWHProjDistFar;    /* Frame size and project distance and far clip */
  VEC4 TimeGlobalDeltaGlobal; /* Time/global time, delta time/global delta time */
  VEC4 Wireframe;             /* Wireframe flag */
} db6RND_UBO_MATR;

/* Primitive's vertex representation type */
typedef struct tagdb6VERTEX
{
  VEC P;  /* Vertex position */
  VEC2 T; /* Texture coordinates */
  VEC N;  /* Normal */
  VEC4 C; /* Color (R, G, B, A)*/
} db6VERTEX;

/* Type of primitives representation type */
typedef enum
{
  DB6_RND_PRIM_TRIMESH,  /* Primitive of triangle (GL_TRIANGLES) */
  DB6_RND_PRIM_TRISTRIP, /* Primitive of triangle strip (GL_TTRIANGLES_STRIP) */
  DB6_RND_PRIM_LINES,    /* Primitive of lines (GL_LINES) */
  DB6_RND_PRIM_POINTS,   /* Primitive of points (GL_POINTS) */
  DB6_RND_PRIM_PATCH     /* Patch primitive (GL_PATCHES) */
} db6PRIM_TYPE;

/* Primitive representation type */
typedef struct tagdb6PRIM
{
  db6PRIM_TYPE Type; /* Primitive type */
  INT
    VA, VBuf,         /* Array and buffer of vertexes */
    IBuf,             /* Buffer of indexes */
    NumOfElements,    /* Number of indexes */
    NumOfPatchPoints; /* Number of patch points */

  MATR Trans; /* Additional transformation matrix */

  VEC MinBB, MaxBB; /* 3d-size of primitive */

  INT MtlNo; /* Material number in material array */
} db6PRIM;

/* Group of primitives representation type */
typedef struct tagdb6PRIMS
{
  db6PRIM *Prims; /* Array of primitives */
  INT NumOfPrims; /* Number of primitives */
  MATR Trans;     /* Common transformation matrix */
  VEC MinBB, MaxBB;
} db6PRIMS;

/* Grid topology representation type */
typedef struct tagdb6GRIND
{
  INT W, H;     /* Grid size (in vertexes) */
  db6VERTEX *V; /* Array of vertexes */
} db6GRID;

/***
 * Rendering system functions
 ***/

/* Render initialization function.
 * ARGUMENTS:
 *   - handle of window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID DB6_RndInit( HWND hWnd );

/* Render close function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndClose( VOID );

/* Render resizing function.
 * ARGUMENTS:
 *   - handle of window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID DB6_RndResize( INT W, INT H );

/* Render redraw frame function.
 * ARGUMENTS:
 * RETURNS: None.
 */
VOID DB6_RndCopyFrame( VOID );

/* Start render function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndStart( VOID );

/* Render initialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndEnd( VOID );

/* Set render projection function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndProjSet( VOID );

/* Set render camera function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndCamSet( VEC Loc, VEC At, VEC Up );

/***
 * Grid primitive functions
 ***/

/* Create grind function.
 * ARGUMENTS:
 *   - grid data:
 *       db6GRID *G;
 *   - grid size:
  *      INT W, H.
 * RETURNS:
 *   (BOOL) Success;
 */
BOOL DB6_RndGridCreate( db6GRID *G, INT W, INT H );
/* Grid free function.
 * ARGUMENTS:
 *   - grid data:
 *       db6GRID *G.
 * RETURNS: None.
 */
VOID DB6_RndGridFree( db6GRID *G );

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       db6GRID *G.
 * RETURNS: None.
 */
VOID DB6_RndGridAutoNormals( db6GRID *G );

/* Set sphere grid by radius and grid size function.
 * ARGUMENTS:
 *   - primitive data
 *       db6PRIM *Pr;
 *   - grid data
 *       db6GRID *G;
 * RETURNS: None.
 */
VOID DB6_RndPrimFromGrid( db6PRIM *Pr, db6GRID *G );

/* Set sphere grid by radius and grid size function.
 * ARGUMENTS:
 *   - grid data
 *       db6GRID *G;
 *   - radius:
 *       DBL R;
 *   - grid size:
 *       INT W, H.
 * RETURNS:
 *   (BOOL) Success of creation.
 */
BOOL DB6_RndGridCreateSphere( db6GRID *G, DBL R, INT W, INT H );

/***
 * Primitive functions
 ***/

/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       db6PRIM *Pr;
 *   - type of primitive:
 *       db6PRIM_TYPE Type;
 *   - number of vertexes:
 *       INT NoofV;
 *   - number of indexes:
 *       INT NoofI.
 * RETURNS: None.
 */
VOID DB6_RndPrimCreate( db6PRIM *Pr, db6PRIM_TYPE Type, db6VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

/* Free memory for primitive function.
 * ARGUMENTS:
 *   - primitive array:
 *       db6PRIM *Pr.
 * RETURNS: None.
 */      
VOID DB6_RndPrimFree( db6PRIM *Pr );

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       db6PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DB6_RndPrimLoad( db6PRIM *Pr, CHAR *FileName );

/* Draw primetive function.
 * ARGUMENTS:
 *   - primitive array:
 *       db6PRIM *Pr;
 *   - world matrix:
 *       MATR World;
 * RETURNS: None.
 */      
VOID DB6_RndPrimDraw( db6PRIM *Pr, MATR World );

/* Set sphere primitive by radius and grinds count function.
 * ARGUMENTS:
 *   - pointer to primitive
 *       db6PRIM *Pr;
 *   - radius of sphere:
 *       DBL R;
 * RETURNS: None.
 */
VOID DB6_RndPrimSetSphere( db6PRIM *Prm, DBL R, INT ScaleH, INT ScaleW );

/* Set torus primitive by radiuses and grinds count function.
 * ARGUMENTS:
 *   - pointer to primitive
 *       db6PRIM *Pr;
 *   - big and small radiuses of torus:
 *       DBL R1, R2;
 * RETURNS: None.
 */
VOID DB6_RndPrimSetTorus( db6PRIM *Pr, DBL R1, DBL R2, INT GrindsW, INT GrindsH );

/***
 * Group of primitives functions
 ***/

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOFPrimitives;
 * RETURNS:
 *   (BOOL) Success.
 */
BOOL DB6_RndPrimsCreate( db6PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DB6_RndPrimsFree( db6PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DB6_RndPrimsDraw( db6PRIMS *Prs, MATR World );

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) Success.
 */
BOOL DB6_RndPrimsLoad( db6PRIMS *Prs, CHAR *FileName );

/***
 * Render target
 ***/

/* Render targets initialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetInit( VOID );

/* Render targets deinitialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetClose( VOID );

/* create render targets function.
 * ARGUMENTS:
 *   - frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID DB6_RndTargetCreate( INT W, INT H );

/* Delete render targets function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetFree( VOID );

/* Start rendering targets function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetStart( VOID );

/* End rendering targets function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTargetEnd( VOID );

/* Resize render targets buffers function.
 * ARGUMENTS:
 *   - frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID DB6_RndTargetResize( INT W, INT H );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */
