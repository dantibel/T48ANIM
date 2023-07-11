/* FILE NAME: input.c
 * PURPOSE: 3D animation project.
 *          Animation system implementation module.
 *          Input system implementation.
 * PROGRAMMER: DB6
 * DATE: 14.01.2022
 */
#include "anim/anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

#include <string.h>

/* Get joystic axis macros */
#define DB6_GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) - 1)

/* Keyboard initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID DB6_AnimKeyboardInit( VOID )
{
  memset(DB6_Anim.Keys, 0, 256);
  memset(DB6_Anim.KeysOld, 0, 256);
  memset(DB6_Anim.KeysClick, 0, 256);
} /* End of 'DB6_AnimKeyboardInit' function */

/* Responcing keyboard function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID DB6_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(DB6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    DB6_Anim.Keys[i] >>= 7;
    DB6_Anim.KeysClick[i] = DB6_Anim.Keys[i] && !DB6_Anim.KeysOld[i];
  }
  memcpy(DB6_Anim.KeysOld, DB6_Anim.Keys, 256);
} /* End of 'DB6_AnimKeyboardResponse' function */


/* Initialization of mouse function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID DB6_AnimMouseInit( VOID )
{
  DB6_Anim.Mdx = DB6_Anim.Mdy = DB6_Anim.Mdz = DB6_Anim.Mx = DB6_Anim.My = DB6_Anim.Mz = 0;
  DB6_Anim.MouseWheel = 0;
} /* End of 'DB6_AnimMouseInit' function */

/* Responcing mouse function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID DB6_AnimMouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(DB6_Anim.hWnd, &pt);

  DB6_Anim.Mdx = pt.x - DB6_Anim.Mx;
  DB6_Anim.Mdy = pt.y - DB6_Anim.My;
  DB6_Anim.Mx = pt.x;
  DB6_Anim.My = pt.y;

  DB6_Anim.Mdz = DB6_Anim.MouseWheel;
  DB6_Anim.Mz += DB6_Anim.MouseWheel;
  DB6_Anim.MouseWheel = 0;
} /* End of 'DB6_AnimMouseResponse' function */

static VOID DB6_AnimJoystickInit( VOID )
{
  memset(DB6_Anim.JBut, 0, 32);
  memset(DB6_Anim.JButClick, 0, 32);
  memset(DB6_Anim.JButOld, 0, 32);
  DB6_Anim.JPov = DB6_Anim.JR = DB6_Anim.JX = DB6_Anim.JY = DB6_Anim.JZ = 0;
} /* End of 'DB6_AnimJoystickInit' function */

static VOID DB6_AnimJoystickResponse( VOID )
{
  INT i;

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          DB6_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          DB6_Anim.JButClick[i] = DB6_Anim.JBut[i] && !DB6_Anim.JButOld[i];
          DB6_Anim.JButOld[i] = DB6_Anim.JBut[i];
        }
        /* Axes */
        DB6_Anim.JX = DB6_GET_JOYSTIC_AXIS(X);
        DB6_Anim.JY = DB6_GET_JOYSTIC_AXIS(Y);
        DB6_Anim.JZ = DB6_GET_JOYSTIC_AXIS(Z);
        DB6_Anim.JR = DB6_GET_JOYSTIC_AXIS(R);
 
        /* Point of view */ 
        DB6_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}

VOID DB6_AnimInputInit( VOID )
{
  DB6_AnimMouseInit();
  DB6_AnimKeyboardInit();
  DB6_AnimJoystickInit();
} /* End of 'DB6_AnimInputInit' function */

VOID DB6_AnimInputResponse( VOID )
{
  DB6_AnimMouseResponse();
  DB6_AnimKeyboardResponse();
  DB6_AnimJoystickResponse();
} /* End of 'DB6_AnimInputResponse' function */

/* END OF 'input.c' FILE */
