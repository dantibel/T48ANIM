/* FILE NAME: anim.c
 * PURPOSE: 3D animation project.
 *          Animation system implementation module.
 *          Animation functions implementation.
 * PROGRAMMER: DB6
 * DATE: 21.01.2022
 */
#include <stdio.h>

#include "anim/anim.h"

db6ANIM DB6_Anim; /* Animation context struct */

/* Initialization of animation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS: None.
 */
VOID DB6_AnimInit( HWND hWnd )
{
  memset(&DB6_Anim, 0, sizeof(db6ANIM));

  DB6_Anim.hWnd = hWnd;
  DB6_RndInit(hWnd);
  DB6_Anim.W = DB6_RndFrameW;
  DB6_Anim.H = DB6_RndFrameH;
  DB6_Anim.IsWireframe = FALSE;
  DB6_AnimTimerInit();
  DB6_AnimInputInit();
} /* End of 'DB6_AnimInit' function */

/* Close animation function.
 * ARGUMENTS:None;
 * RETURNS: None.
 */
VOID DB6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < DB6_Anim.NumOfUnits; i++)
  {
    DB6_Anim.Units[i]->Close(DB6_Anim.Units[i], &DB6_Anim);
    free(DB6_Anim.Units[i]);
    DB6_Anim.Units[i] = NULL;
  }
  DB6_Anim.NumOfUnits = 0;
  DB6_RndClose();
  PostQuitMessage(30);
} /* End of 'DB6_AnimClose' function */

/* Set new size of animation function.
 * ARGUMENTS:
 *   - screen size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID DB6_AnimResize( INT W, INT H )
{
  DB6_Anim.W = W;
  DB6_Anim.H = H;
  DB6_RndResize(W, H);
} /* End of 'DB6_AnimResize' function */

/* Copy frame of animation function.
 * ARGUMENTS:
 * RETURNS: None.
 */
VOID DB6_AnimCopyFrame( VOID )
{
  DB6_RndCopyFrame();
} /* End of 'DB6_AnimCopyFrame' function */

/* Render animation frame function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimRender( VOID )
{
  INT i;
  static DBL SaveTime;

  /* Timer request */
  DB6_AnimTimerResponse();

  /* Input devices request */
  //if (DB6_Anim.IsActive)
    DB6_AnimInputResponse();

  /* Units response */
  for (i = 0; i < DB6_Anim.NumOfUnits; i++)
    DB6_Anim.Units[i]->Response(DB6_Anim.Units[i], &DB6_Anim);

  /* Units render */
  DB6_RndStart();
  for (i = 0; i < DB6_Anim.NumOfUnits; i++)
    DB6_Anim.Units[i]->Render(DB6_Anim.Units[i], &DB6_Anim);
  DB6_RndEnd();
} /* End of 'DB6_AnimRender' function */

/* Copy frame of animation function.
 * ARGUMENTS:
     - handle of divice context;
 * RETURNS: None.
 */
VOID DB6_AnimUnitAdd( db6UNIT *Uni )
{
  if (DB6_Anim.NumOfUnits < DB6_MAX_UNITS)
  {
    DB6_Anim.Units[DB6_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &DB6_Anim);
  }
} /* End of 'DB6_AnimUnitAdd' funtion */

/* Flip window to full screen mode function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* Store current mode */
  static RECT SaveRC;               /* Save old window rectangle */

  if (IsFullScreen)
  {
    IsFullScreen = FALSE;

    /* Restore window position and size */
    SetWindowPos(DB6_hRndWnd, HWND_NOTOPMOST, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left,
      SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;

    IsFullScreen = TRUE;
 
    /* Save window rectangle */
    GetWindowRect(DB6_hRndWnd, &SaveRC);

    /* Get closest monitor */
    hMon = MonitorFromWindow(DB6_hRndWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(DB6_hRndWnd, GWL_STYLE), FALSE);

    /* Expand window client area to full screen */
    SetWindowPos(DB6_hRndWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'DB6_AnimFlipFullScreen' function */
/* END OF 'anim.c' FILE */
