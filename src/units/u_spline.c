 /* PURPOSE: 3D animation project.
 *          Spline unit's methods implementation module.
 * PROGRAMMER: DB6
/* FILE NAME: u_spline.c
 * PURPOSE: 3D animation project.
 *          Spline unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 21.02.2022
 */
#include "units.h"

/* Maximum number of spline points */
#define DB6_SPLINE_MAX_POINTS 8

typedef struct tagdb6UNIT_SPLINE
{
  DB6_UNIT_BASE_FIELDS;               /* Basic unit methods */
  db6PRIM Pr;                         /* Spline primitive */
  db6VERTEX P[DB6_SPLINE_MAX_POINTS]; /* Points vertexes */
  VEC V[DB6_SPLINE_MAX_POINTS];       /* Points velocity */
  db6PRIM Sph;                        /* Sphere primitive */
} db6UNIT_SPLINE;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_SPLINE *Uni, db6ANIM *Ani )
{
  INT i;
  db6MATERIAL mtl = DB6_RndMtlGetDef();
  db6VERTEX V[(DB6_SPLINE_MAX_POINTS - 1) * 4] = {{{0}}};

  for (i = 0; i < DB6_SPLINE_MAX_POINTS; i++)
  {
    Uni->P[i].P = VecSet(Rnd1(), Rnd1() + 20, Rnd1());
    Uni->V[i] = VecSet(Rnd1(), Rnd1(), Rnd1());
  }

  for (i = 0; i < DB6_SPLINE_MAX_POINTS - 1; i++)
  {
    V[4 * i + 0] = Uni->P[i == 0 ? 0 : i - 1];
    V[4 * i + 1] = Uni->P[i];
    V[4 * i + 2] = Uni->P[i + 1];
    V[4 * i + 3] = Uni->P[i == DB6_SPLINE_MAX_POINTS - 2 ? DB6_SPLINE_MAX_POINTS - 1 : i + 2];
  }

  DB6_RndPrimCreate(&Uni->Pr, DB6_RND_PRIM_PATCH, V, (DB6_SPLINE_MAX_POINTS - 1) * 4, NULL, 0);
  Uni->Pr.NumOfPatchPoints = 4;
  strncpy(mtl.Name, "Spline material", DB6_STR_MAX);
  mtl.ShdNo = DB6_RndShdAdd("tess_line");
  Uni->Pr.MtlNo = DB6_RndMtlAdd(&mtl);

  DB6_RndPrimCreate(&Uni->Sph, DB6_RND_PRIM_PATCH, NULL, 1, NULL, 0);
  Uni->Sph.NumOfPatchPoints = 1;
  strncpy(mtl.Name, "Tess sphere material", DB6_STR_MAX);
  mtl.ShdNo = DB6_RndShdAdd("tess_sphere");
  Uni->Sph.MtlNo = DB6_RndMtlAdd(&mtl);

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
static VOID DB6_UnitClose( db6UNIT_SPLINE *Uni, db6ANIM *Ani )
{
  DB6_RndPrimFree(&Uni->Pr);
  DB6_RndPrimFree(&Uni->Sph);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_SPLINE *Uni, db6ANIM *Ani )
{
  INT i;
  db6VERTEX V[DB6_SPLINE_MAX_POINTS * 4] = {{{0}}};

  DB6_RndShdAddonI[0] += Ani->KeysClick['E'] - Ani->KeysClick['D'];

  for (i = 0; i < DB6_SPLINE_MAX_POINTS; i++)
  {
    Uni->P[i].P = VecAddVec(Uni->P[i].P, VecMulNum(Uni->V[i], Ani->DeltaTime));

    /* Clip with reflect */
    if (Uni->P[i].P.X < -1)
      Uni->V[i].X = fabs(Uni->V[i].X);
    if (Uni->P[i].P.X > 1)
      Uni->V[i].X = -fabs(Uni->V[i].X);
    if (Uni->P[i].P.Y < 19)
      Uni->V[i].Y = fabs(Uni->V[i].Y);
    if (Uni->P[i].P.Y > 21)
      Uni->V[i].Y = -fabs(Uni->V[i].Y);
    if (Uni->P[i].P.Z < -1)
      Uni->V[i].Z = fabs(Uni->V[i].Z);
    if (Uni->P[i].P.Z > 1)
      Uni->V[i].Z = -fabs(Uni->V[i].Z);
  }

  for (i = 0; i < DB6_SPLINE_MAX_POINTS - 1; i++)
  {
    V[4 * i + 0] = Uni->P[i == 0 ? 0 : i - 1];
    V[4 * i + 1] = Uni->P[i];
    V[4 * i + 2] = Uni->P[i + 1];
    V[4 * i + 3] = Uni->P[i == DB6_SPLINE_MAX_POINTS - 2 ? DB6_SPLINE_MAX_POINTS - 1 : i + 2];
  }

  glBindBuffer(GL_ARRAY_BUFFER, Uni->Pr.VBuf);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(db6VERTEX) * (DB6_SPLINE_MAX_POINTS - 1) * 4, V);
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
static VOID DB6_UnitRender( db6UNIT_SPLINE *Uni, db6ANIM *Ani )
{
  INT i;

  for (i = 0; i < DB6_SPLINE_MAX_POINTS; i++)
    DB6_RndPrimDraw(&Uni->Sph, MatrTranslate(Uni->P[i].P));
  DB6_RndPrimDraw(&Uni->Pr, MatrIdentity());
} /* End of 'DB6_UnitRender' function */

/* Spline unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSpline( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_SPLINE))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateSpline' function */

/* END OF 'u_spline.c' FILE */
