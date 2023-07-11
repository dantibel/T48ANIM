/* FILE NAME: u_g3dm.c
 * PURPOSE: 3D animation project.
 *          Bounce ball unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 04.02.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_G3DM
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIMS Prs;
} db6UNIT_G3DM;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_G3DM *Uni, db6ANIM *Ani )
{
  DB6_RndPrimsLoad(&Uni->Prs, "bin/models/helicopter.g3dm");
  Uni->Prs.Trans = MatrMulMatr4(MatrScale(VecSet1(0.01)), MatrRotateZ(20), MatrRotateX(20), MatrTranslate(VecSet(30, 100, 0)));
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_G3DM *Uni, db6ANIM *Ani )
{
  DB6_RndPrimsFree(&Uni->Prs);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_G3DM *Uni, db6ANIM *Ani )
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
static VOID DB6_UnitRender( db6UNIT_G3DM *Uni, db6ANIM *Ani )
{
  DB6_RndPrimsDraw(&Uni->Prs, MatrRotateY(Ani->Time * -10));
} /* End of 'DB6_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateG3DM( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_G3DM))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateBBal' function */

/* END OF 'u_g3dm.c' FILE */
