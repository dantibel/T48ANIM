/* FILE NAME: u_light.c
 * PURPOSE: 3D animation project.
 *          Light unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 21.01.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_LIGHT
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Lamp;         /* Light source */
  VEC Dir;              /* Light's position */
} db6UNIT_LIGHT;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_LIGHT *Uni, db6ANIM *Ani )
{
  VEC d = VecMulNum(Uni->Dir = VecNormalize(VecSet(1, 1, 1)), 14);

  DB6_RndPrimSetSphere(&Uni->Lamp, 1, 10, 10);
  Uni->Lamp.Trans = MatrTranslate(VecSet(d.X, d.Y * .75, d.Z));
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_LIGHT *Uni, db6ANIM *Ani )
{
  DB6_RndPrimFree(&Uni->Lamp);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_LIGHT *Uni, db6ANIM *Ani )
{
  if (Ani->IsPause)
    return;
  Uni->Dir = VecMulMatr(Uni->Dir, MatrRotateY(100 * Ani->DeltaTime));
  Uni->Lamp.Trans = MatrMulMatr(MatrRotateY(100 * Ani->DeltaTime), Uni->Lamp.Trans);
} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT_LIGHT *Uni, db6ANIM *Ani )
{
  DB6_RndPrimDraw(&Uni->Lamp, Uni->Lamp.Trans);
} /* End of 'DB6_UnitRender' function */

/* Unit light creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateLight( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_LIGHT))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateLight' function */

/* END OF 'u_light.c' FILE */
