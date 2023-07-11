/* FILE NAME: rndgrid.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Grid implementtion.
 * PROGRAMMER: DB6
 * DATE: 31.01.2022
 */
#include "anim/anim.h"

/* Create grind function.
 * ARGUMENTS:
 *   - grid data:
 *       db6GRID *G;
 *   - grid size:
  *      INT W, H;
 * RETURNS:
 *   (BOOL) Success;
 */
BOOL DB6_RndGridCreate( db6GRID *G, INT W, INT H )
{
  INT i, j;

  memset(G, 0, sizeof(db6GRID));

  /* Memory allocation */
  if ((G->V = malloc(W * H * sizeof(db6VERTEX))) == NULL)
    return FALSE;

  memset(G->V, 0, W * H * sizeof(db6VERTEX));
  G->W = W, G->H = H;
  for (i = 0; i < H; i++)
    for (j = 0; j < W; j++)
    {
      G->V[i * W + j].P = VecSet(j, 0, -i);
      G->V[i * W + j].N = VecSet(0, 1, 0);
      G->V[i * W + j].T.X = j / (W - 1.0);
      G->V[i * W + j].T.Y = i / (H - 1.0);
    }

  return TRUE;
} /* End of 'DB6_RndGridCreate' function */

/* Grid free function.
 * ARGUMENTS:
 *   - grid data:
 *       db6GRID *G;
 * RETURNS: None.
 */
VOID DB6_RndGridFree( db6GRID *G )
{
  if (G->V != NULL)
    free(G->V);
  memset(G, 0, sizeof(db6GRID));
} /* End of 'DB6_RndGridFree' function */

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       db6GRID *G;
 * RETURNS: None.
 */
VOID DB6_RndGridAutoNormals( db6GRID *G )
{
  INT i, j;

  /* Reset all normals */
  for (i = 0; i < G->H * G->W; i++)
    G->V[i].N = VecSet1(0);

  /* Eval all facets normals */
  for (i = 0; i < G->H - 1; i++)
    for (j =  0; j < G->W - 1; j++)
    {
      db6VERTEX
        *P00 = G->V + i * G->W + j,
        *P01 = G->V + i * G->W + j + 1,
        *P10 = G->V + (i + 1) * G->W + j,
        *P11 = G->V + (i + 1) * G->W + j + 1;
      VEC N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                       VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }

  /* Normalize all normals */
  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = VecNormalize(G->V[i].N);
} /* End of 'DB6_RndGridAutoNormals' function */

/* Set sphere grid by radius and grid size function.
 * ARGUMENTS:
 *   - primitive data
 *       db6PRIM *Pr;
 *   - grid data
 *       db6GRID *G;
 * RETURNS: None.
 */
VOID DB6_RndPrimFromGrid( db6PRIM *Pr, db6GRID *G )
{
  INT i, j, k, *I;

  memset(Pr, 0, sizeof(db6PRIM));
  if ((I = malloc(sizeof(INT) * ((G->H - 1) * (G->W * 2 + 1) - 1))) == NULL)
    return;

  for (i = 0, k = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      I[k++] = (i + 1) * G->W + j;
      I[k++] = i * G->W + j;
    }
    if (i != G->H - 2)
      I[k++] = -1;
  }
  DB6_RndPrimCreate(Pr, DB6_RND_PRIM_TRISTRIP, G->V, G->W * G->H, I, (G->H - 1) * (G->W * 2 + 1) - 1);
  free(I);
} /* End of 'DB6_RndPrimFromGrid' function */

/* Set sphere grid by radius and grid size function.
 * ARGUMENTS:
 *   - grid data
 *       db6GRID *G;
 *   - radius:
 *       DBL R;
 *   - grid size:
 *       INT W, H.
 * RETURNS:
 *   (BOOL) Success of creation.
 */
BOOL DB6_RndGridCreateSphere( db6GRID *G, DBL R, INT W, INT H )
{
  INT i, j, k;
  DBL theta, phi;

  if (!DB6_RndGridCreate(G, W, H))
    return FALSE;

  /* Set vertexes coordinates and normals */
  for (i = 0, k = 0, theta = 0; i < H; i++, theta += PI / ( - 1))
    for (j = 0, phi = 0; j < W; j++, k++, phi += 2 * PI /(W - 1))
    {
      G->V[k].N = VecSet(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));
      G->V[k].P = VecMulNum(G->V[k].N, R);
    }

  return TRUE;
} /* End of 'DB6_RndGridCreateSphere' function */

/* END OF 'rndgrid.c' FILE */
