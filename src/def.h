/* FILE NAME: def.h
 * PURPOSE: 3D animation project.
 *          Common declaration module.
 * PROGRAMMER: DB6
 * DATE: 04.02.2022
 */
 
#ifndef __def_h_
#define __def_h_

#pragma warning(disable: 4244)

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "mth/mth.h"

/* Debug output function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - elipsys: ...
 * RETURNS:
 *   (INT) number of output characters.
 */
__inline INT DB6_Debug( CHAR *Fmt, ... )
{
  static CHAR Buf[1000];
  INT ret;
  va_list ap;

  va_start(ap, Fmt);
  ret = vsprintf(Buf, Fmt, ap);
  OutputDebugString(Buf);
  return ret;
} /* End of 'DB6_Debug' function */

/* 64 bit integer data type */
typedef long long INT64;
typedef unsigned long long UINT64;

#endif /* __def_h_ */
/* END OF 'def.h' FILE */
