/* FILE NAME: rndres.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementation module.
 *          Resources implementation.
 * PROGRAMMER: DB6
 * DATE: 21.01.2022
 */
#include "rndres.h"

/* Resource initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndResInit( VOID )
{
  DB6_RndShdInit();
  DB6_RndTexInit();
  DB6_RndMtlInit();
  DB6_RndFntInit();
}

/* Resource deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndResClose( VOID )
{
  DB6_RndShdFree(DB6_RndShdProgId);
} /* End of 'DB6_RndShdFree' function */

/* END OF 'rndres.c' FILE */