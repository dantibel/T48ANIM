/* FILE NAME: u_sky.c
 * PURPOSE: 3D animation project.
 *          Grid unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 18.02.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_GRID
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Sky;          /* Sky sphere */
} db6UNIT_GRID;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_GRID *Uni, db6ANIM *Ani )
{
  db6MATERIAL mtl = DB6_RndMtlGetDef();

  /* Set sky sphere primitive */
  DB6_RndPrimCreate(&Uni->Sky, DB6_RND_PRIM_POINTS, NULL, 1, NULL, 0);

  strncpy(mtl.Name, "Sky material", DB6_STR_MAX);
  mtl.Tex[0] = DB6_RndTexAddFromFile("bin/textures/sky_sphere.bmp");
  mtl.Kd = VecSet1(0.8);
  mtl.Ks = VecSet1(0);
  mtl.ShdNo = DB6_RndShdAdd("sky");
  Uni->Sky.MtlNo = DB6_RndMtlAdd(&mtl);
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_GRID *Uni, db6ANIM *Ani )
{
  DB6_RndPrimFree(&Uni->Sky);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_GRID *Uni, db6ANIM *Ani )
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
static VOID DB6_UnitRender( db6UNIT_GRID *Uni, db6ANIM *Ani )
{
  glDepthMask(FALSE);
  DB6_RndPrimDraw(&Uni->Sky, MatrIdentity());
  glDepthMask(TRUE);
} /* End of 'DB6_UnitRender' function */

/* Unit gr creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSky( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_GRID))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateSnow' function */

/* END OF 'u_sky.c' FILE */
