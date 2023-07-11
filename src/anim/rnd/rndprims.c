/* FILE NAME: rndprims.h
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Primitive functions implementation.
 * PROGRAMMER: DB6
 * DATE: 04.02.2022
 */
#include <stdio.h>

#include "anim/rnd/rnd.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOFPrimitives;
 * RETURNS:
 *   (BOOL) Success.
 */
BOOL DB6_RndPrimsCreate( db6PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(db6PRIMS));
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  if ((Prs->Prims = malloc(NumOfPrims * sizeof(db6PRIM))) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(db6PRIM) * NumOfPrims);
  return TRUE;
} /* End of 'DB6_RndPrimsCreate' function */

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 * RETURNS: None.
 */
VOID DB6_RndPrimsFree( db6PRIMS *Prs )
{
  INT i;

  if (Prs->Prims != NULL)
    for (i = 0; i < Prs->NumOfPrims; i++)
      DB6_RndPrimFree(&Prs->Prims[i]);
  free(Prs->Prims);
  memset(Prs, 0, sizeof(db6PRIMS));
} /* End of 'DB6_RndPrimsFree' function */

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DB6_RndPrimsDraw( db6PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->Trans, World);

  /* Draw all nontransparent primitives */
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    db6MATERIAL *mtl = DB6_RndMtlGet(Prs->Prims[i].MtlNo);

    if (mtl->Trans == 1)
      DB6_RndPrimDraw(&Prs->Prims[i], m);
  }

  /* Draw all transparent primitives */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    db6MATERIAL *mtl = DB6_RndMtlGet(Prs->Prims[i].MtlNo);

    if (mtl->Trans != 1)
      DB6_RndPrimDraw(&Prs->Prims[i], m);
  }
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
  {
    db6MATERIAL *mtl = DB6_RndMtlGet(Prs->Prims[i].MtlNo);

    if (mtl->Trans != 1)
      DB6_RndPrimDraw(&Prs->Prims[i], m);
  }
  glDisable(GL_CULL_FACE);
} /* End of 'DB6_RndPrimsDraw' function */

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       db6PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) Success.
 */
BOOL DB6_RndPrimsLoad( db6PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, i, j;
  BYTE *mem, *ptr;
  DWORD Sign, NumOfPrims, NumOfMaterials, NumOfTextures;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Obtain file lenth */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  /* Allocate memeory and load whole file to memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(mem, 1, flen, F);
  fclose(F);

  /* Load file information */
  ptr = mem;
  Sign = *(DWORD *)ptr, ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr, ptr += 4;
  NumOfMaterials = *(DWORD *)ptr, ptr += 4;
  NumOfTextures = *(DWORD *)ptr, ptr += 4;
  if (!DB6_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }

  /* Load primitives */
  for (i = 0; i < NumOfPrims; i++)
  {
    INT *I;
    DWORD NumOfVertexes, NumOfFacetIndexes, MtlNo;
#pragma pack(push, 1)
    struct
    {
       VEC P;  /* Vertex position */
       VEC2 T; /* Texture coordinates */
       VEC N;  /* Normal */
       VEC4 C; /* Color (r,g,b,a) */
    } *v_file;
#pragma pack(pop)
    db6VERTEX *V;

    NumOfVertexes = *(DWORD *)ptr, ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr, ptr += 4;
    MtlNo = *(DWORD *)ptr + DB6_RndMaterialsSize, ptr += 4;
    v_file = (VOID *)ptr, ptr += sizeof(v_file[0]) * NumOfVertexes;

    if ((V = malloc(NumOfVertexes * sizeof(db6VERTEX))) == NULL || (I = malloc(NumOfFacetIndexes * sizeof(INT))) == NULL)
    {
      DB6_RndPrimsFree(Prs);
      free(mem);
      return FALSE;
    }

    for (j = 0; j < NumOfVertexes; j++)
    {
      V[j].P= v_file[j].P;
      V[j].T= v_file[j].T;
      V[j].N= v_file[j].N;
      V[j].C= v_file[j].C;
    }

    DB6_RndPrimCreate(&Prs->Prims[i], DB6_RND_PRIM_TRIMESH, V, NumOfVertexes, (INT *)ptr, NumOfFacetIndexes);
    ptr += sizeof(INT) * NumOfFacetIndexes;
    free(V);
    free(I);
    Prs->Prims[i].MtlNo = MtlNo;

    if (i == 0)
      Prs->MinBB = Prs->Prims[i].MinBB, Prs->MaxBB = Prs->Prims[i].MaxBB;
    else
    {
      if (Prs->MinBB.X > Prs->Prims[i].MinBB.X)
        Prs->MinBB.X = Prs->Prims[i].MinBB.X;
      if (Prs->MaxBB.X < Prs->Prims[i].MaxBB.X)
        Prs->MaxBB.X = Prs->Prims[i].MaxBB.X;

      if (Prs->MinBB.Y > Prs->Prims[i].MinBB.Y)
        Prs->MinBB.Y = Prs->Prims[i].MinBB.Y;
      if (Prs->MaxBB.Y < Prs->Prims[i].MaxBB.Y)
        Prs->MaxBB.Y = Prs->Prims[i].MaxBB.Y;

      if (Prs->MinBB.Z > Prs->Prims[i].MinBB.Z)
        Prs->MinBB.Z = Prs->Prims[i].MinBB.Z;
      if (Prs->MaxBB.Z < Prs->Prims[i].MaxBB.Z)
        Prs->MaxBB.Z = Prs->Prims[i].MaxBB.Z;
    }
  }

  /* Load materials */
  for (i = 0; i< NumOfMaterials; i++)
  {
#pragma pack(push, 1)
    struct
    {
      CHAR Name[DB6_STR_MAX];         /* Material name */
      VEC Ka, Kd, Ks;                 /* Ambient, diffuse, specula, coefficients */
      FLT Ph;                         /* Phong power coefficient - shiniess */
      FLT Trans;                      /* Tranparency factor */
      DWORD Tex[8];                   /* Texture references (texture number in G3DM file) */
      CHAR ShaderString[DB6_STR_MAX]; /* Additional shader information */
      DWORD Shader;                   /* Shader number (uses after load into memory) */
    } *fmat;
#pragma pack(pop)
    db6MATERIAL mtl;

    fmat = (VOID *)ptr, ptr += sizeof(*fmat);
    strncpy(mtl.Name, fmat->Name, DB6_STR_MAX);
    mtl.Ka = fmat->Ka;
    mtl.Kd = fmat->Kd;
    mtl.Ks = fmat->Ks;
    mtl.Ph = fmat->Ph;
    mtl.Trans = fmat->Trans;
    mtl.ShdNo = fmat->Shader;
    for (j = 0; j < 8; j++)
      mtl.Tex[j] = fmat->Tex[j] == -1 ? -1 : DB6_RndTexturesSize + fmat->Tex[j];
    DB6_RndMtlAdd(&mtl);
  }

  /* Load textures */
  for (i = 0; i < NumOfTextures; i++)
  {
    CHAR *Name;
    DWORD W, H, C;

    Name = (CHAR *)ptr, ptr += 300;
    W = *(DWORD *)ptr, ptr += 4;
    H = *(DWORD *)ptr, ptr += 4;
    C = *(DWORD *)ptr, ptr += 4;
    DB6_RndTexAddImg(Name, W, H, C, ptr), ptr += W * H * C;
  }

  free(mem);
  return TRUE;
} /* End of 'DB6_RndPrimsLoad' function */

/* END OF 'rndprims.c' FILE */
