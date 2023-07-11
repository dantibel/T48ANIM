/* FILE NAME: anim.h
 * PURPOSE: 3D animation project.
 *          Animation system declaration module.
 * PROGRAMMER: DB6
 * DATE: 17.01.2022
 */
#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

/* Maximum count of units */
#define DB6_MAX_UNITS 1000

/* Basic unit functions */
#define DB6_UNIT_BASE_FIELDS \
  VOID (*Init)( db6UNIT *Uni, db6ANIM *Ani );     \
  VOID (*Close)( db6UNIT *Uni, db6ANIM *Ani );    \
  VOID (*Response)( db6UNIT *Uni, db6ANIM *Ani ); \
  VOID (*Render)( db6UNIT *Uni, db6ANIM *Ani )

/* Animation units representation type */
typedef struct tagdb6UNIT db6UNIT;

/* Animation context representation type */
typedef struct tagdb6ANIM
{
  HWND hWnd;    /* Windows */
  INT W, H;     /* Screen size */
  BOOL IsActive; /* Is window active */

  db6UNIT *Units[DB6_MAX_UNITS]; /* array of animation units */
  INT NumOfUnits;                /* Number of units */

  DBL
    GlobalTime,      /* Time since program start */
    GlobalDeltaTime, /* Time since last frame update */
    Time,            /* Time since program start with pauses */
    DeltaTime,       /* Time since last frame update with pauses */
    FPS;             /* Frames per seconds */
  BOOL IsPause;      /* Is pause mode on flag */
  
  BYTE Keys[256];      /* Keys statement */
  BYTE KeysOld[256];   /* Old keys statement */
  BYTE KeysClick[256]; /* Array of click's signs */

  INT
    Mx, My, Mz,     /* Mouse coordinates */
    Mdx, Mdy, Mdz,  /* Delta mouse coordinates */
    MouseWheel; /* Mouse wheel rotating counter */

  BYTE
    JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */

  BOOL IsWireframe; /* Wireframe flag */
} db6ANIM;

extern db6ANIM DB6_Anim; /* Animation context struct */

/* Animation units representation type */
struct tagdb6UNIT
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
};

/* Initialization of animation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS: None.
 */
VOID DB6_AnimInit( HWND hWnd );

/* Close animation function.
 * ARGUMENTS:None;
 * RETURNS: None.
 */
VOID DB6_AnimClose( VOID );

/* Set new size of animation function.
 * ARGUMENTS:
 *   - screen size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID DB6_AnimResize( INT W, INT H );

/* Copy frame of animation function.
 * ARGUMENTS:
 * RETURNS: None.
 */
VOID DB6_AnimCopyFrame( VOID );

/* Render animation frame function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimRender( VOID );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_AnimUnitCreate( INT Size );

/* Copy frame of animation function.
 * ARGUMENTS:
     - handle of divice context;
 * RETURNS: None.
 */
VOID DB6_AnimUnitAdd( db6UNIT *Uni );

/* Timer initialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimTimerInit( VOID );

/* Measure time and FPS function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimTimerResponse( VOID );

/* Flip window to full screen mode function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimFlipFullScreen( VOID );

extern INT DB6_MouseWheel; /* Mouse wheel rotating counter */

VOID DB6_AnimInputInit( VOID );

VOID DB6_AnimInputResponse( VOID );
#endif /* __anim_h_ */
/* END OF 'anim.h' FILE */
