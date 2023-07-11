/* FILE NAME: u_smoke.c
 * PURPOSE: 3D animation project.
 *          Smoke unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 13.02.2022
 */
#include <stdlib.h>

#include "units.h"


typedef struct tagdb6UNIT_SMOKE
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Pr;       /* Primitive */
} db6UNIT_SMOKE;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_SMOKE *Uni, db6ANIM *Ani )
{
  db6MATERIAL mtl = DB6_RndMtlGetDef();

  DB6_RndPrimCreate(&Uni->Pr, DB6_RND_PRIM_POINTS, NULL, 64, NULL, 0);

  Uni->Pr.Trans = MatrIdentity();

  strncpy(mtl.Name, "Smoke material", DB6_STR_MAX);
  mtl.Tex[0] = DB6_RndTexAddFromFile("bin/textures/smoke.g32");
  mtl.Kd = VecSet1(0.8);
  mtl.Ks = VecSet1(0);
  mtl.ShdNo = DB6_RndShdAdd("smoke");
  Uni->Pr.MtlNo = DB6_RndMtlAdd(&mtl);
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_SMOKE *Uni, db6ANIM *Ani )
{
  DB6_RndPrimFree(&Uni->Pr);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_SMOKE *Uni, db6ANIM *Ani )
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
static VOID DB6_UnitRender( db6UNIT_SMOKE *Uni, db6ANIM *Ani )
{
  glDepthMask(FALSE);
  DB6_RndPrimDraw(&Uni->Pr, MatrIdentity());
  glDepthMask(TRUE);
} /* End of 'DB6_UnitRender' function */

/* Unit smoke creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSmoke( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_SMOKE))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateSmoke' function */

/* END OF 'u_smoke.c' FILE */
