/* FILE NAME: u_grid.c
 * PURPOSE: 3D animation project.
 *          Grid unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 11.02.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_GRID
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Pr;       /* Primitive */
  db6PRIM Sky;   /* Sky sphere */
} db6UNIT_GRID;

/* Global height map store (bitmap, height and size)*/
static HBITMAP hBmHeightField;
static BITMAP BmHeightField;
static FLT HeightFieldH = 250, HeightFieldSize = 2000;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
FLT DB6_UnitgridgetHeight( VEC Pos )
{
  FLT x, y;

  if (hBmHeightField == NULL)
  {
    hBmHeightField = LoadImage(NULL, "in/textures/hmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBmHeightField  == NULL)
      return 0;
    GetObject(hBmHeightField, sizeof(BmHeightField), &BmHeightField);
  }

  x = (BmHeightField.bmWidth - 1.0) * (Pos.X + HeightFieldSize * 0.5) / HeightFieldSize;
  y = (BmHeightField.bmHeight - 1.0) * (-Pos.Z + HeightFieldSize * 0.5) / HeightFieldSize;
  x = COM_MIN(COM_MAX(0, x), BmHeightField.bmWidth - 1);
  y = COM_MIN(COM_MAX(0, y), BmHeightField.bmHeight - 1);
  return 0;
  //return *(((BYTE *)BmHeightField.bmBits + BmHeightField.bmWidthBytes * y + x) * HeightFieldH / HeightFieldSize);
} /* End of 'DB6_UnitgridgetHeight' function*/

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
  db6GRID G;
  HBITMAP hBm;

  /* Set ground */
  if ((hBm = LoadImage(NULL, "bin/textures/hmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP bm;
    INT x, y;

    GetObject(hBm, sizeof(bm), &bm);
    DB6_RndGridCreate(&G, bm.bmWidth, bm.bmHeight);

    for (y = 0; y < bm.bmHeight; y++)
      for (x = 0; x < bm.bmWidth; x++)
      {
        BYTE c = *((BYTE *)bm.bmBits + bm.bmWidthBytes * y + x);
        G.V[y * G.W + x].P = VecSet(HeightFieldSize * x / (bm.bmWidth - 1.0) - 0.5 * HeightFieldSize, HeightFieldH * c / 255.0,
                                    HeightFieldSize * (1 - y /(bm.bmHeight - 1.0)) - 0.5 * HeightFieldSize);
      }
    DB6_RndGridAutoNormals(&G);
    DB6_RndPrimFromGrid(&Uni->Pr, &G);
    Uni->Pr.Trans = MatrIdentity();
    DB6_RndGridFree(&G);

    DeleteObject(hBm);
  }

  if ((hBm = LoadImage(NULL, "bin/textures/hmap_tex.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP bm;

    GetObject(hBm, sizeof(bm), &bm);
    mtl.Tex[0] = DB6_RndTexAddImg("Height map", bm.bmWidth, bm.bmHeight, bm.bmBitsPixel / 8, bm.bmBits);
    DeleteObject(hBm);
  }
  strncpy(mtl.Name, "Grid material", DB6_STR_MAX);
  mtl.Kd = VecSet1(0.8);
  mtl.Ks = VecSet1(0.1);
  mtl.Ka = VecSet1(0.1);
  Uni->Pr.MtlNo = DB6_RndMtlAdd(&mtl);

  /* Set sky sphere */
  /*
  DB6_RndGridCreateSphere(&G, 10, 30, 20);
  DB6_RndPrimFromGrid(&Uni->Sky, &G);
  DB6_RndGridFree(&G);

  strncpy(mtl.Name, "Sky material", DB6_STR_MAX);
  mtl.Tex[0] = DB6_RndTexAddFromFile("sky_sphere.bmp");
  mtl.Kd = VecSet1(0.8);
  mtl.Ks = VecSet1(0);
  Uni->Sky.MtlNo = DB6_RndMtlAdd(&mtl);
  */
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
  DB6_RndPrimFree(&Uni->Pr);
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
  DB6_RndPrimDraw(&Uni->Pr, MatrIdentity());
  //DB6_RndPrimDraw(&Uni->Sky, MatrIdentity());
} /* End of 'DB6_UnitRender' function */

/* Unit grid creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateGrid( VOID )
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
} /* End of 'DB6_UnitCreateCar' function */

/* END OF 'u_grid.c' FILE */
