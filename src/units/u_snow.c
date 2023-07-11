/* FILE NAME: u_snow.c
 * PURPOSE: 3D animation project.
 *          Snow unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 9.02.2022
 */
#include <stdlib.h>

#include "units.h"

/* Snow flakes count */
#define DB6_MAX_FLAKES 300

typedef struct tagdb6UNIT_SNOW
{
  DB6_UNIT_BASE_FIELDS;                 /* Basic unit methods */
  db6PRIM Pr;                       /* Primitive */
  db6VERTEX Flakes[DB6_MAX_FLAKES]; /* Snow flakes coordinates */
} db6UNIT_SNOW;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_SNOW *Uni, db6ANIM *Ani )
{
  INT i;
  db6MATERIAL mtl = DB6_RndMtlGetDef();

  /* Fill snow flakes */
  for (i = 0; i < DB6_MAX_FLAKES; i++)
    Uni->Flakes[i].P = VecSet(Rnd1() * 100, Rnd0() * 100, Rnd1() * 100);

  DB6_RndPrimCreate(&Uni->Pr, DB6_RND_PRIM_POINTS, Uni->Flakes, DB6_MAX_FLAKES, NULL, 0);

  Uni->Pr.Trans = MatrIdentity();

  strncpy(mtl.Name, "Snow material", DB6_STR_MAX);
  mtl.Tex[0] = DB6_RndTexAddFromFile("bin/textures/snow3.g32");
  mtl.Kd = VecSet1(0.8);
  mtl.Ks = VecSet1(0);
  mtl.ShdNo = DB6_RndShdAdd("snow");
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
static VOID DB6_UnitClose( db6UNIT_SNOW *Uni, db6ANIM *Ani )
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
static VOID DB6_UnitResponse( db6UNIT_SNOW *Uni, db6ANIM *Ani )
{
  INT i;

  if (Ani->IsPause)
    return;
  for (i = 0; i < DB6_MAX_FLAKES; i++)
  {
    Uni->Flakes[i].P.Y -= Ani->DeltaTime;
    if (Uni->Flakes[i].P.Y <= 20)
      Uni->Flakes[i].P.Y += 100;
  }
  glBindBuffer(GL_ARRAY_BUFFER, Uni->Pr.VBuf);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(db6VERTEX) * DB6_MAX_FLAKES, Uni->Flakes);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT_SNOW *Uni, db6ANIM *Ani )
{
  DB6_RndPrimDraw(&Uni->Pr, MatrIdentity());
} /* End of 'DB6_UnitRender' function */

/* Unit snow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSnow( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_SNOW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateSnow' function */

/* END OF 'u_gsnow.c' FILE */
