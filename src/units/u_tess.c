/* FILE NAME: u_tess.c
 * PURPOSE: 3D animation project.
 *          Tessellated unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 18.02.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_TESS
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Pr, Sph;      /* Tessellated grid primitives */
} db6UNIT_TESS;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_TESS *Uni, db6ANIM *Ani )
{
  db6MATERIAL mtl = DB6_RndMtlGetDef();
  db6VERTEX V[16] =
  {
    {{-3, 3, -2}}, {{-1, -1, -2}}, {{1, 1, -2}}, {{3, -3, -2}},
    {{-3, 3, 0}}, {{-1, -1, 0}}, {{1, 1, 0}}, {{3, -3, 0}},
    {{-3, 3, 2}}, {{-1, -1, 2}}, {{1, 1, 2}}, {{3, -3, 2}},
    {{-3, 3, 4}}, {{-1, -1, 4}}, {{1, 1, 4}}, {{3, -3, 4}}
  };

  DB6_RndPrimCreate(&Uni->Pr, DB6_RND_PRIM_PATCH, V, 16, NULL, 0);
  Uni->Pr.NumOfPatchPoints = 4;
  strncpy(mtl.Name, "Tess grid material", DB6_STR_MAX);
  mtl.ShdNo = DB6_RndShdAdd("tess_grid");
  Uni->Pr.MtlNo = DB6_RndMtlAdd(&mtl);

  DB6_RndPrimCreate(&Uni->Pr, DB6_RND_PRIM_PATCH, NULL, 1, NULL, 0);
  Uni->Pr.NumOfPatchPoints = 1;
  strncpy(mtl.Name, "Tess sphere material", DB6_STR_MAX);
  mtl.ShdNo = DB6_RndShdAdd("tess_sphere");
  Uni->Pr.MtlNo = DB6_RndMtlAdd(&mtl);

  DB6_RndShdAddonI[0] = 20;
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_TESS *Uni, db6ANIM *Ani )
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
static VOID DB6_UnitResponse( db6UNIT_TESS *Uni, db6ANIM *Ani )
{
  DB6_RndShdAddonI[0] += Ani->KeysClick['E'] - Ani->KeysClick['D'];
} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT_TESS *Uni, db6ANIM *Ani )
{
  db6VERTEX V[16] =
  {
    {{-3, 3, -2}}, {{-1, -1, -2}}, {{1, 1, -2}}, {{3, -3, -2}},
    {{-3, 3, 0}}, {{-1, -1, 0}}, {{1, 1, 0}}, {{3, -3, 0}},
    {{-3, 3, 2}}, {{-1, -1, 2}}, {{1, 1, 2}}, {{3, -3, 2}},
    {{-3, 3, 4}}, {{-1, -1, 4}}, {{1, 1, 4}}, {{3, -3, 4}}
  };
  INT i;

  for (i = 0; i < 16; i++)
    DB6_RndPrimDraw(&Uni->Sph, MatrTranslate(V[i].P));
  DB6_RndPrimDraw(&Uni->Pr, MatrIdentity());
} /* End of 'DB6_UnitRender' function */

/* Tessellated unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateTess( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_TESS))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateCam' function */

/* END OF 'u_tess.c' FILE */
