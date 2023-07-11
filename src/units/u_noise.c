/* FILE NAME: u_noise.c
 * PURPOSE: 3D animation project.
 *          Noise grid unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#include <stdlib.h>
#include <math.h>
#include "units.h"

/***
 * Noise implementation
 ***/

/* Noise table size and constants */
#define TAB_BITS 8
#define TAB_SIZE (1 << TAB_BITS)
#define TAB_MASK (TAB_SIZE - 1)

/* Get index macroses */
#define INDEX1(X) (Perm[(X) & TAB_MASK])
#define INDEX2(X, Y) (Perm[(X + INDEX1(Y)) & TAB_MASK])
#define INDEX3(X, Y, Z) (Perm[(X + INDEX2(Y, Z)) & TAB_MASK])
#define INDEX4(X, Y, Z, W) (Perm[(X + INDEX3(Y, Z, W)) & TAB_MASK])

/* Noise table */
DBL TabNoise[TAB_SIZE];

/* Permutation table */
INT Perm[TAB_SIZE];

/* Noise table initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID NoiseInit( VOID )
{
  INT i;

  for (i = 0; i < TAB_SIZE; i++)
    TabNoise[i] = (DBL)rand() / RAND_MAX;

  for (i = 0; i < TAB_SIZE; i++)
    Perm[i] = i;

  for (i = 0; i < TAB_SIZE * 8; i++)
  {
    INT a = rand() / RAND_MAX, b = rand() / RAND_MAX, tmp = Perm[a];

    Perm[a] = Perm[b];
    Perm[b] = tmp;
  }
} /* End of 'NoiseInit' function */

/* Noise 1D obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       DBL X;
 * RETURNS:
 *   (DBL) noise value.
 */
static DBL Noise1D( DBL X )
{
  INT ix = floor(X), ix1;
  DBL fx;

  fx = X - ix;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;
  return TabNoise[ix] * (1 - fx) + TabNoise[ix1] * fx;
} /* End of 'Noise1D' function */

/* Turbulence noise 1D obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       DBL X;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (DBL) noise value.
 */
static DBL NoiseTurb1D( DBL X, INT Octaves )
{
  INT i, fact = 1;
  DBL val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise1D(X) / fact;
    X = (X + 28.47) * 2;
    fact *= 2;
  }
  return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
} /* End of 'Noiseturb1D' function */

/* Turbulence noise 1D obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       DBL X;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (VEC2) noise value.
 */
static VEC2 Noise2Turb1D( DBL X, INT Octaves )
{
  VEC2 val = { NoiseTurb1D(X, Octaves), NoiseTurb1D(X + 47.28, Octaves) };

  return val;
} /* End of 'Noise2Turb1D' function */

/* Noise 2D obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       DBL X, Y;
 * RETURNS:
 *   (DBL) noise value.
 */
static DBL Noise2D( DBL X, DBL Y )
{
  INT ix = floor(X), ix1, iy = floor(Y), iy1;
  DBL fx, fy;

  fx = X - ix;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;
  fy = Y - iy;
  iy &= TAB_MASK;
  iy1 = (iy + 1) & TAB_MASK;

  return
    TabNoise[INDEX2(iy, ix)] * (1 - fx) * (1 - fy) +
    TabNoise[INDEX2(iy, ix1)] * fx * (1 - fx) +
    TabNoise[INDEX2(iy1, ix)] * (1 - fx) * fy +
    TabNoise[INDEX2(iy1, ix1)] * fx * fy;
} /* End of 'Noise2D' function */

/* Turbulence noise 2D obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       DBL X, Y;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (DBL) noise value.
 */
static DBL NoiseTurb2D( DBL X, DBL Y, INT Octaves )
{
  INT i, fact = 1;
  DBL val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise2D(X, Y) / fact;
    X = (X + 28.47) * 2, Y = (Y + 24.78) * 2;
    fact *= 2;
  }
  return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
} /* End of 'Noiseturb2D' function */

/* Turbulence noise 2D obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       DBL X, Y;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (VEC2) noise value.
 */
static VEC2 Noise2Turb2D( DBL X, DBL Y, INT Octaves )
{
  VEC2 val = { NoiseTurb2D(X, Y, Octaves), NoiseTurb2D(X + 28.47, Y + 24.78, Octaves) };

  return val;
} /* End of 'Noise2Turb1D' function */

/***
 * Unit methods implementation
 ***/

typedef struct tagdb6UNIT_NOISE
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Pr;           /* Primitive */
  db6GRID G;            /* Grid */
  INT Oct;              /* Number of octaves */
} db6UNIT_NOISE;

/* Size of noise grid */
#define DB6_GRID_SIZE 100

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_NOISE* Uni, db6ANIM* Ani )
{
  INT x, y;
  FLT HeightFieldSize = 30;
  db6MATERIAL mtl = DB6_RndMtlGetDef();

  srand(14.05);
  if (DB6_RndGridCreate(&Uni->G, DB6_GRID_SIZE, DB6_GRID_SIZE))
  {
    for (y = 0; y < DB6_GRID_SIZE; y++)
      for (x = 0; x < DB6_GRID_SIZE; x++)
      {
        Uni->G.V[y * Uni->G.W + x].P = VecSet(HeightFieldSize * x / (DB6_GRID_SIZE - 1.0) - 0.5 * HeightFieldSize,
                                              1,
                                              HeightFieldSize * (1 - y / (DB6_GRID_SIZE - 1.0)) - 0.5 * HeightFieldSize);
        //Uni->G.V[y * Uni->G.W + x].P.Y = 100 * Ani->DeltaTime * NoiseTurb2D(rand() / (x + 1), rand() / (y + 1), Uni->Oct);
        Uni->G.V[y * Uni->G.W + x].C = Vec4Set1(0.6);
      }
    DB6_RndGridAutoNormals(&Uni->G);
    DB6_RndPrimFromGrid(&Uni->Pr, &Uni->G);
    Uni->Pr.Trans = MatrIdentity();
  }
  //Uni->Pr.Type = DB6_RND_PRIM_PATCH;
  //Uni->Pr.NumOfPatchPoints = 4;
  strncpy(mtl.Name, "Noise material", DB6_STR_MAX);
  mtl.Ka = VecSet1(0.1);
  mtl.Kd = VecSet1(0.7);
  mtl.ShdNo = 0;
  Uni->Pr.MtlNo = DB6_RndMtlAdd(&mtl);
  Uni->Oct = 1;
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_NOISE* Uni, db6ANIM* Ani )
{
  DB6_RndPrimFree(&Uni->Pr);
  DB6_RndGridFree(&Uni->G);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_NOISE* Uni, db6ANIM* Ani )
{
  INT x, y;

  if (Ani->KeysClick[VK_ADD] && Uni->Oct < 10)
    Uni->Oct++;
  if (Ani->KeysClick[VK_SUBTRACT] && Uni->Oct > 1)
     Uni->Oct--;

  for (y = 0; y < DB6_GRID_SIZE; y++)
    for (x = 0; x < DB6_GRID_SIZE; x++)
    {
      if (!Ani->IsPause)
        Uni->G.V[y * Uni->G.W + x].P.Y = 30 * Ani->DeltaTime * NoiseTurb2D(rand() + 17.37, rand() + 19.29, Uni->Oct);
    }
  DB6_RndGridAutoNormals(&Uni->G);

  glBindBuffer(GL_ARRAY_BUFFER, Uni->Pr.VBuf);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(db6VERTEX) * DB6_GRID_SIZE * DB6_GRID_SIZE, Uni->G.V);
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
static VOID DB6_UnitRender(db6UNIT_NOISE* Uni, db6ANIM* Ani)
{
  DB6_RndPrimDraw(&Uni->Pr, MatrIdentity());
  //DB6_RndPrimDraw(&Uni->Sky, MatrIdentity());
} /* End of 'DB6_UnitRender' function */

/* Unit noise grid creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT* DB6_UnitCreateNoise( VOID )
{
  db6UNIT* Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_NOISE))) == NULL)
    return NULL;

  NoiseInit();

  /* Setup unit methods */
  Uni->Init = (VOID*)DB6_UnitInit;
  Uni->Response = (VOID*)DB6_UnitResponse;
  Uni->Render = (VOID*)DB6_UnitRender;
  Uni->Close = (VOID*)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateCar' function */

/* END OF 'u_noise.c' FILE */
