/* FILE NAME: rndprim.h
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Primitive functions implementation.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#include <stdio.h>
#include <string.h>

#include "anim/anim.h"

/* Create primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       db6PRIM *Pr;
 *   - type of primitive:
 *       db6PRIM_TYPE Type;
 *   - number of vertexes:
 *       INT NumOfV;
 *   - number of indexes:
 *       INT NumOfI.
 * RETURNS: None.
 */
VOID DB6_RndPrimCreate( db6PRIM *Pr, db6PRIM_TYPE Type, db6VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  INT i;

  memset(Pr, 0, sizeof(db6PRIM));

  Pr->Type = Type;

  glGenVertexArrays(1, &Pr->VA);

  if (V != NULL)
  {
    glGenBuffers(1, &Pr->VBuf);

    /* activate array of vertexes */
    glBindVertexArray(Pr->VA);

    /* Activate buffer */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    /* Put data (NumOfV - count of vertexes, V - array of vertexes) */
    glBufferData(GL_ARRAY_BUFFER, sizeof(db6VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    /* Add at array of vertexes buffer with data */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(db6VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(db6VERTEX),
                          (VOID *)sizeof(VEC)); /* textere's coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(db6VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(db6VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    /* Off array of vertexes */
    glBindVertexArray(0);
  }

  if (NumOfI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);

    /* Activate buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);

    /* Add at array of vertexes buffer with data */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    Pr->NumOfElements = NumOfI;

    /* Activate buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  }
  else
    Pr->NumOfElements = NumOfV;

  /* Set MinBB and MaxBB */
  Pr->MinBB = Pr->MaxBB = VecSet1(0);
  if (V != NULL)
  {
    if (NumOfV != 0)
      Pr->MinBB = Pr->MaxBB = V[0].P;
    for (i = 1; i < NumOfV; i++)
    {
      if (Pr->MinBB.X > V[i].P.X)
        Pr->MinBB.X = V[i].P.X;
      if (Pr->MaxBB.X < V[i].P. X)
        Pr->MaxBB.X = V[i].P.X;

      if (Pr->MinBB.Y > V[i].P.Y)
        Pr->MinBB.Y = V[i].P.Y;
      if (Pr->MaxBB.Y < V[i].P.Y)
        Pr->MaxBB.Y = V[i].P.Y;

      if (Pr->MinBB.Z > V[i].P.Z)
        Pr->MinBB.Z = V[i].P.Z;
      if (Pr->MaxBB.Z < V[i].P.Z)
        Pr->MaxBB.Z = V[i].P.Z;
    }
  }

  Pr->Trans = MatrIdentity();
} /* End of 'DB6_RndPrimCreate' function */

/* Free memory for primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to free:
 *       db6PRIM *Pr.
 * RETURNS: None.
 */
VOID DB6_RndPrimFree( db6PRIM *Pr )
{
  memset(Pr, 0, sizeof(db6PRIM));

  if (Pr->VA != 0)
  {
    /* Activate vertex array */
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (Pr->VBuf != 0)
      glDeleteBuffers(1, &Pr->VBuf);

    /* Deactivate vertex array */
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);
  }
  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->VBuf);

  memset(Pr, 0, sizeof(db6PRIM));
} /* End of 'DB6_RndPrimFree' function */

/* Draw primetive function.
 * ARGUMENTS:
 *   - pointer to primitive to draw:
 *       db6PRIM *Pr;
 *   - world space matrix:
 *       MATR World.
 * RETURNS: None.
 */
VOID DB6_RndPrimDraw( db6PRIM *Pr, MATR World )
{
  INT i, loc, ProgId, gl_prim_type = Pr->Type == DB6_RND_PRIM_LINES ? GL_LINES :
                                     Pr->Type == DB6_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                                     Pr->Type == DB6_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
                                     Pr->Type == DB6_RND_PRIM_PATCH ? GL_PATCHES : GL_POINTS;
  MATR
    w = MatrMulMatr(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w)),
    wvp = MatrMulMatr(w, DB6_RndMatrVP);

  ProgId = DB6_RndMtlApply(Pr->MtlNo);

  /* Send render parametrs to shader */
  DB6_RndUBOUpdate(DB6_RndUBOCamera,
    (INT)(&((db6RND_UBO_MATR *)0)->MatrW) / 16, 4, w.A[0]);
  DB6_RndUBOUpdate(DB6_RndUBOCamera,
    (INT)(&((db6RND_UBO_MATR *)0)->MatrWInv) / 16, 4, winv.A[0]);
  DB6_RndUBOUpdate(DB6_RndUBOCamera,
    (INT)(&((db6RND_UBO_MATR *)0)->MatrWVP) / 16, 4, wvp.A[0]);
  for (i = 0; i < sizeof(DB6_RndShdAddonI) / sizeof(DB6_RndShdAddonI[0]); i++)
  {
    CHAR Name[10] = "AddonInt0";

    Name[8] = i + '0';
    if ((loc = glGetUniformLocation(ProgId, Name)) != -1)
      glUniform1i(loc, DB6_RndShdAddonI[i]);
  }
  for (i = 0; i < sizeof(DB6_RndShdAddonF) / sizeof(DB6_RndShdAddonF[0]); i++)
  {
    CHAR Name[10] = "AddonFlt0";

    Name[8] = i + '0';
    if ((loc = glGetUniformLocation(ProgId, Name)) != -1)
      glUniform1f(loc, DB6_RndShdAddonF[i]);
  }

  /* Activate array of vertexes */
  glBindVertexArray(Pr->VA);

  if (gl_prim_type == GL_PATCHES)
    glPatchParameteri(GL_PATCH_VERTICES, Pr->NumOfPatchPoints);

  if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
  else
  {
    /* Activate array of indexes */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);

    /* Drawing primitive */
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);

    /* Deactivate array of indexes */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  /* Deactivate array of vertexes */
  glBindVertexArray(0);
} /* End of 'DB6_RndPrimDraw' function */

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       db6PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName.
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DB6_RndPrimLoad( db6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, ni = 0, *I, i, NormI = 0;
  VEC Light = {1, 0, 1};
  db6VERTEX *V;
  static CHAR Buf[10000];

  memset(Pr, 0, sizeof(db6PRIM));
  Pr->Trans = MatrIdentity();

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      CHAR *S = Buf + 2, oldc = ' ';
      INT cnt = 0;

      while (*S != 0)
        cnt += isspace((UCHAR)oldc) && !isspace((UCHAR)*S), oldc = *S++;
      ni += cnt - 2;
    }
  }

  /* Memory allocation */
  if ((V = malloc(sizeof(db6VERTEX) * nv + sizeof(INT) * ni * 3)) == NULL)
    return FALSE;
  I = (INT *)(V + nv);

  /* Read model data */
  rewind(F);
  nv = 0;
  ni = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      CHAR *S = Buf + 2, oldc = ' ';
      INT n = 0, n0 = 0, n1 = 0, nc;

      while (*S != 0)
      {
        if (isspace((UCHAR)oldc) && !isspace((UCHAR)*S))
        {
          sscanf(S, "%d", &nc);
          if (nc > 0)
            nc--;
          else
            nc += nv;
          if (n == 0)
            n0 = nc;
          else if (n == 1)
            n1 = nc;
          else
          {
            I[ni++] = n0;
            I[ni++] = n1;
            I[ni++] = nc;
            n1 = nc;
          }
          n++;
        }
        oldc = *S++;
      }
    }
  }
  fclose(F);

  /* Setup all normals */
  for (i = 0; i < nv; i++)
    V[i].N = VecSet1(0);

  /* Eval all facets normals */
  for (i = 0; i < ni; i += 3)
  {
    VEC
      p0 = V[I[i]].P,
      p1 = V[I[i + 1]].P,
      p2 = V[I[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

    V[I[i]].N = VecAddVec(V[I[i]].N, N);
    V[I[i + 1]].N = VecAddVec(V[I[i + 1]].N, N);
    V[I[i + 2]].N = VecAddVec(V[I[i + 2]].N, N);
  }

  /* Normalize all normals */
  for (i = 0; i < nv; i++)
    V[i].N = VecNormalize(V[i].N);

  /* Set color */
  for (i = 0; i < nv; i++)
  {
    V[i].C = Vec4Set(0.1, 0.1, 0.1, 1);
  }

  DB6_RndPrimCreate(Pr, DB6_RND_PRIM_TRIMESH, V, nv, I, ni);

  free(V);
  return TRUE;
} /* End of 'DB6_RndPrimLoad' function */

/* Set sphere primitive by radius and grinds count function.
 * ARGUMENTS:
 *   - pointer to primitive
 *       db6PRIM *Pr;
 *   - radius of sphere:
 *       DBL R;
 * RETURNS: None.
 */
VOID DB6_RndPrimSetSphere( db6PRIM *Pr, DBL R, INT GridsW, INT GridsH )
{
  INT i, j, k = 0, *I, NumOfV = GridsW * GridsH, NumOfI = (GridsW - 1) * (GridsH - 1) * 6;
  DBL theta, phi;
  db6VERTEX *V;

  /* Memory allocation */
  if ((V = malloc(sizeof(db6VERTEX) * NumOfV)) == NULL)
    return;
  if ((I = malloc(sizeof(INT) * NumOfI)) == NULL)
  {
    free(V);
    return;
  }

  /* Set vertexes coordinates and color */
  for (i = 0, theta = 0; i < GridsH; i++, theta += PI / (GridsH - 1))
    for (j = 0, phi = 0; j < GridsW; j++, phi += 2 * PI / (GridsW - 1))
    {
      V[i * GridsW + j].P = VecSet(R * sin(theta) * sin(phi), R * cos(theta), R * sin(theta) * cos(phi));
      V[i * GridsW + j].N = VecSet(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));
      V[i * GridsW + j].C = Vec4Set(1, 1, 1, 1);
    }

  /* Set triangles */
  for (i = 0; i < GridsH - 1; i++)
    for (j = 0; j < GridsW - 1; j++)
    {
      I[k++] = i * GridsW + j;
      I[k++] = i * GridsW + j + 1;
      I[k++] = (i + 1) * GridsW + j;

      I[k++] = (i + 1) * GridsW + j;
      I[k++] = (i + 1) * GridsW + j + 1;
      I[k++] = i * GridsW + j + 1;
    }
  DB6_RndPrimCreate(Pr, DB6_RND_PRIM_TRIMESH, V, NumOfV, I, NumOfI);

  free(V), free(I);
} /* End of 'DB6_RndPrimSetSphere' function */

/* Set torus primitive by radiuses and grinds count function.
 * ARGUMENTS:
 *   - pointer to primitive
 *       db6PRIM *Pr;
 *   - big and small radiuses of torus:
 *       DBL R1, R2;
 * RETURNS: None.
 */
VOID DB6_RndPrimSetTorus( db6PRIM *Pr, DBL R1, DBL R2, INT GridsW, INT GridsH )
{
  INT i, j, k = 0, *I, NumOfV = GridsW * GridsH, NumOfI = (GridsW - 1) * (GridsH - 1) * 6;
  DBL theta, phi;
  db6VERTEX *V;

  if ((V = malloc(sizeof(db6VERTEX) * NumOfV)) == NULL)
    return;
  if ((I = malloc(sizeof(INT) * NumOfI)) == NULL)
  {
    free(V);
    return;
  }

  /* Set torus coordinates */
  for (i = 0, theta = 0; i < GridsH; i++, theta += 2 * PI / (GridsH - 1))
    for (j = 0, phi = 0; j < GridsW; j++, phi += 2 * PI /(GridsW - 1))
    {
      V[i * GridsW + j].P = VecSet((R1 + R2 * cos(theta)) * cos(phi),
      (R1 + R2 * cos(theta)) * sin(phi), R2 * sin(theta));
      //V[i * GridsW + j].C = Vec4Set1(255);
    }

  /* Set triangles */
  for (i = 0; i < GridsH - 1; i++)
    for (j = 0; j < GridsW - 1; j++)
    {
      I[k++] = i * GridsW + j;
      I[k++] = i * GridsW + j + 1;
      I[k++] = (i + 1) * GridsW + j;

      I[k++] = (i + 1) * GridsW + j;
      I[k++] = (i + 1) * GridsW + j + 1;
      I[k++] = i * GridsW + j + 1;
    }
  DB6_RndPrimCreate(Pr, DB6_RND_PRIM_TRIMESH, V, NumOfV, I, NumOfI);
} /* End of 'DB6_RndPrimSetTorus' function */

/* Set plates primitive by radiuses and grinds count function.
 * ARGUMENTS:
 *   - pointer to primitive
 *       db6PRIM *Pr;
 *   - big and small radiuses of torus:
 *       DBL R1, R2;
 * RETURNS: None.
 */
VOID DB6_RndPrimSetPlate( db6PRIM *Pr, INT SizeW, INT SizeH, INT GridsW, INT GridsH )
{
  INT i, j, k = 0, step_w = SizeW / GridsW, step_h = SizeH / GridsH;

  //if (!DB6_RndPrimCreate(Pr, SizeW * SizeH, (SizeW - 1) * (SizeH - 1) * 6))
    //return;

  /* Set plate coordinates */
  for (i = 0; i < SizeH; i += step_w)
    for (j = 0; j < SizeW; j += step_h)
      //Pr->V[i * SizeW + j].P = VecSet(i, 0, j);

  /* Set triangles */
  for (i = 0; i < SizeH; i += step_w)
    for (j = 0; j < SizeW; j += step_h)
    {
      /*
      Pr->I[k++] = i * SizeW + j;
      Pr->I[k++] = i * SizeW + j + step_w;
      Pr->I[k++] = (i + step_h) * SizeW + j;

      Pr->I[k++] = (i + step_h) * SizeW + j;
      Pr->I[k++] = (i + step_h) * SizeW + j + step_w;
      Pr->I[k++] = i * SizeW + j + step_w;
      */
    }
} /* End of 'PrimSetPlate' function */

/* END OF 'rndprim.c' FILE */
