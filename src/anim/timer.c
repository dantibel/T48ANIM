/* FILE NAME: timer.c
 * PURPOSE: 3D animation project.
 *          Animation system implementation module.
 *          Timer implementation.
 * PROGRAMMER: DB6
 * DATE: 13.01.2022
 */
#include "anim.h"

static UINT64 TimePerSec;
static UINT64 StartTime, OldTime, OldTimeFPS, PauseTime, FrameCounter;

/* Timer initialization function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimTimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  DB6_Anim.IsPause = FALSE;
  DB6_Anim.FPS = 30.0;
  PauseTime = 0;
  DB6_Anim.GlobalTime = DB6_Anim.GlobalDeltaTime = DB6_Anim.Time = DB6_Anim.DeltaTime = DB6_Anim.FPS = 0;
  DB6_Anim.IsPause = FALSE;
} /* End of 'DB6_AnimTimerInit' function */

/* Measure time and FPS function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_AnimTimerResponse( VOID )
{
  LARGE_INTEGER t;
 
  QueryPerformanceCounter(&t);

  /* Global time */
  DB6_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  DB6_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (DB6_Anim.IsPause)
  {
    DB6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    DB6_Anim.DeltaTime = DB6_Anim.GlobalDeltaTime;
    DB6_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }

  /* FPS measure */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    DB6_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'TimerResponse' function */

/* END OF 'timer.c' FILE */
