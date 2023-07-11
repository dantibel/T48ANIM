/* FILE NAME: unit.c
 * PURPOSE: 3D animation project.
 *          Animation system implementation module.
 *          Animation unit's basic methods implementation.
 * PROGRAMMER: DB6
 * DATE: 10.01.2022
 */         
#include "anim/anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT *Uni, db6ANIM *Ani )
{
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT *Uni, db6ANIM *Ani )
{
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT *Uni, db6ANIM *Ani )
{
} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT *Uni, db6ANIM *Ani )
{
} /* End of 'DB6_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_AnimUnitCreate( INT Size )
{
  db6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(db6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;

  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = DB6_UnitInit;
  Uni->Close = DB6_UnitClose;
  Uni->Response = DB6_UnitResponse;
  Uni->Render = DB6_UnitRender;

  return Uni;
} /* End of 'DB6_AnimUnitCreate' function */
/* END OF 'units.c' FILE */