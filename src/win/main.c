/* FILE NAME: main.c
 * PURPOSE: 3D animation project.
 *          Work with window module.
 * PROGRAMMER: DB6
 * LAST UPDATE: 17.01.2022
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "units/units.h"

/* Window class name */
#define WND_CLASS_NAME "My Winter Camp Main Window Class Name"


/* Forward reference */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  SetDbgMemHooks();
  /* Fill window class description structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;      /* Window class style (see CS_***) */
  wc.cbClsExtra = 0;                       /* Extra class memory size in bytes */
  wc.cbWndExtra = 0;                       /* Extra window memory size in bytes */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Background brush */
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* Cursor handle */
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);   /* Icon handle */
  wc.lpszMenuName = NULL;                  /* Menu resource name */
  wc.hInstance = hInstance;                /* Instance handle */
  wc.lpfnWndProc = MyWindowFunc;           /* Window message handle functionpointer  */
  wc.lpszClassName = WND_CLASS_NAME;       /* Window class name */
 
  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR",
      MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd =
    CreateWindow(WND_CLASS_NAME, /* Window class name */
      "T48ANIM",                 /* Window caption */
      WS_OVERLAPPEDWINDOW,       /* Window styles */
      0, 0,                      /* Window position */
      960, 1080,                  /* Window size */
      NULL,                      /* Owner window handle */
      NULL,                      /* Menu handle */
      hInstance,                 /* Instance handle */
      NULL);                     /* Pointer to window extra data */

  /* Show and update window */
  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  //DB6_AnimUnitAdd(DB6_UnitCreateSky());
  DB6_AnimUnitAdd(DB6_UnitCreateCtrl());
  //DB6_AnimUnitAdd(DB6_UnitCreateCar());
  DB6_AnimUnitAdd(DB6_UnitCreateGrid());
  DB6_AnimUnitAdd(DB6_UnitCreateG3DM());
  DB6_AnimUnitAdd(DB6_UnitCreateSnow());
  //DB6_AnimUnitAdd(DB6_UnitCreateSmoke());
  //DB6_AnimUnitAdd(DB6_UnitCreateTess());
  DB6_AnimUnitAdd(DB6_UnitCreateSpline());
  DB6_AnimUnitAdd(DB6_UnitCreateNoise());




  /* Main message loop (wait for 'WM_QUIT' message */
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
    {
      DB6_AnimRender();
      DB6_AnimCopyFrame();
    }
  }
  return msg.wParam;
} /* End of 'WinMain' function */

/* Window message handle function (CALLBACK version).
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier (see WM_***):
 *       UINT Msg;
 *   - message first parameter ('word' param):
 *       WPARAM wParam;
 *   - message second parameter ('long' param):
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type).
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  MINMAXINFO *minmax;
  PAINTSTRUCT ps;
  static DBL Angle1 = 0, Angle2 = 0, Scale = 1;
  static db6PRIM Pr1, Pr2, Pr3, Pr4;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION)
      + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    DB6_AnimInit(hWnd);
    SetTimer(hWnd, 101, 1, NULL);
    return 0;
  case WM_SIZE:
    DB6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    DB6_AnimRender();
    DB6_AnimCopyFrame();
    return 0;
  case WM_TIMER:
    DB6_AnimRender();
    DB6_AnimCopyFrame();
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DB6_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ACTIVATE:
    DB6_Anim.IsActive = LOWORD(wParam) != WA_INACTIVE;
    return 0;
  case WM_ENTERSIZEMOVE:
    DB6_Anim.IsActive = FALSE;
    return 0;
  case WM_EXITSIZEMOVE:
    DB6_Anim.IsActive = TRUE;
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 101);
    DB6_AnimClose();
    PostQuitMessage(30);
    return 0;
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;
  case WM_MOUSEWHEEL:
    DB6_Anim.MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'main.c' FILE */
