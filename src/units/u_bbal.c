/* FILE NAME: u_bball.c
 * PURPOSE: 3D animation project.
 *          Bounce ball unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 21.01.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_BBALL
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM BBall;    /* BBall's primitive */ 
  VEC Pos;          /* BBall's position */
  INT Index;        /* BBall's index */
} db6UNIT_BBALL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_BBALL *Uni, db6ANIM *Ani )
{
  static CallNum = 0;

  DB6_RndPrimSetSphere(&Uni->BBall, 1, 10, 10);
  Uni->Index = CallNum++;
  if (Uni->Index == 1)
    Uni->Pos = VecSet1(12);
  else
    Uni->Pos = VecSet(0, 0, 0);
  Uni->BBall.Trans = MatrTranslate(Uni->Pos);
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_BBALL *Uni, db6ANIM *Ani )
{
  DB6_RndPrimFree(&Uni->BBall);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_BBALL *Uni, db6ANIM *Ani )
{
  if (Ani->IsPause)
    return;
} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT_BBALL *Uni, db6ANIM *Ani )
{
  DB6_RndPrimDraw(&Uni->BBall, MatrRotateY(Ani->DeltaTime * 3));
} /* End of 'DB6_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateBBall( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_BBALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateBBal' function */

/* END OF 'u_bbal.c' FILE */
