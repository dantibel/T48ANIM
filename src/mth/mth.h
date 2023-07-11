/* FILE NAME: mth.h
 * PURPOSE: 3D animation project.
 *          Math functions declaration.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#ifndef __mth_h_
#define __mth_h_

#include <math.h>
#include <stdlib.h>

#include  <windows.h>
#include  <commondf.h>

/* Pi number */
#define PI 3.14159265358979323846

/* Transfer angle in degree to angle in radians */
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))

/* Unit matrix (for 'MatrIdentity' function) */
#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}

/* Multiplication of matrixes macroses (with using 'MatrMulMatr' function) */
#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr((A), (B)), (C))
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr3((A), (B), (C)), (D))
#define MatrMulMatr5(A, B, C, D, E) MatrMulMatr(MatrMulMatr4((A), (B), (C), (D)), (E))
#define MatrMulMatr6(A, B, C, D, E, F) MatrMulMatr(MatrMulMatr5((A), (B), (C), (D), (E)), (F))

/* Basic float types */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Space vector/point representation types */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC;
typedef struct tagVEC2
{
  FLT X, Y; /* Vector coordinates */
} VEC2;
typedef struct tagVEC4
{
  FLT X, Y, Z, W; /* Vector coordinates */
} VEC4;

/* Transformation matrix representation type */
typedef struct tagMATR
{
  FLT A[4][4]; /* Matrix elements */
} MATR;

/***
 * Vector functions
 ***/

/* Set vector by coordinates function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (VEC) vector.
 */
__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC V = {X, Y, Z};

  return V;
} /* End of 'VecSet' function */

/* Set vector with same coordinates function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       FLT A;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet1( FLT A )
{
  return VecSet(A, A, A);
} /* End of 'VecSet1' function */

/* Set vector by coordinates function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       FLT X, Y;
 * RETURNS:
 *   (VEC2) vector.
 */
__inline VEC2 Vec2Set( FLT X, FLT Y )
{
  VEC2 V = {X, Y};

  return V;
} /* End of 'Vec2Set' function */

/* Set vector with same coordinates function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       FLT A;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2Set1( FLT A )
{
  return Vec2Set(A, A);
} /* End of 'Vec2Set1' function */

/* Set vector by coordinates function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       FLT X, Y, Z, W;
 * RETURNS:
 *   (VEC4) vector.
 */
__inline VEC4 Vec4Set( FLT X, FLT Y, FLT Z, FLT W )
{
  VEC4 V = {X, Y, Z, W};

  return V;
} /* End of 'Vec4Set' function */

/* Set vector with same coordinates function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       FLT A;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC4 Vec4Set1( FLT A )
{
  return Vec4Set(A, A, A, A);
} /* End of 'VecSet1' function */

/* Set 4-component vector by 3-components vector function.
 * ARGUMENTS:
 *   - 3-components vector:
 *       VEC V3;
 * RETURNS:
 *   (VEC4) vector.
 */
__inline VEC4 Vec4SetVec3( VEC V3 )
{
  VEC4 V = {V3.X, V3.Y, V3.Z, 0};

  return V;
} /* End of 'Vec4SetVec3' function */

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Sub two vectors function.
 * ARGUMENTS:
 *   - main vector:
 *       VEC V1;
 *   - vector to be sub:
 *       VEC V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

/* Multiply vector and number function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 *   - number:
 *       FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulNum( VEC V, FLT N )
{
  return VecSet(V.X * N, V.Y * N, V.Z * N);
} /* End of 'VecMulNum' function */

/* Multiply vector and number function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 *   - number:
 *       FLT N;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecDivNum( VEC V, FLT N )
{
  return VecSet(V.X / N, V.Y / N, V.Z / N);
} /* End of 'VecDivNum' function */

/* Get negative vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Dot production of two vectors function.
 * ARGUMENTS:
 *   - vectors:
 *       VEC V1, V2;
 * RETURNS:
 *   (FLT) result number.
 */
__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecMulNum' function */

/* Cross production of two vectors function.
 * ARGUMENTS:
 *   - vectors:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result number.
 */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Get square of lenght of vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (FLT) square of lenght of vector.
 */
__inline FLT VecLen2( VEC V )
{
  return VecDotVec(V, V);
} /* End of 'Veclen2' function */

/* Get lenght of vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (FLT) lenght of vector.
 */
__inline FLT VecLen( VEC V )
{
  FLT len = VecLen2(V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'VecLen' function */

/* Normalize vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (VEC) result number.
 */
__inline VEC VecNormalize( VEC V )
{
  FLT len = VecLen(V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, len);
} /* End of 'VecNormalize' function */

/***
 * Matrix functions
 ***/

/* Set matrix by elements function.
 * ARGUMENTS:
 *   - elements of matrix:
 *       FLT A00, A01, A02, A03, A10, A11, A12, A33, A20, A21, A22, A23, A30, A31, A32, A33;
 * RETURNS:
 *   (MATR) matrix.
 */
__inline MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
                      FLT A10, FLT A11, FLT A12, FLT A13,
                      FLT A20, FLT A21, FLT A22, FLT A23,
                      FLT A30, FLT A31, FLT A32, FLT A33 )
{
  MATR m =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };

  return m;
} /* End of 'MatrSet' function */

/* Set Unit matrix function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (MATR) unit matrix.
 */
__inline MATR MatrIdentity( VOID )
{
  MATR I = UnitMatrix;

  return I;
} /* End of 'MatrIdentity' function */

/* Set translate matrix for vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC T;
 * RETURNS:
 *   (MATR) translate matrix.
 */
__inline MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;
  return m;
} /* End of 'MatrTranslate' function */

/* Set scale matrix for vector function.
 * ARGUMENTS:
 *   - vector:
 *       VEC S;
 * RETURNS:
 *   (MATR) scale matrix.
 */
__inline MATR MatrScale( VEC S )
{
  MATR m = UnitMatrix;

  m.A[0][0] = S.X;
  m.A[1][1] = S.Y;
  m.A[2][2] = S.Z;
  return m;
} /* End of 'MatrScale' function */

/* Set rotate by X-vector matrix for vector by angle function.
 * ARGUMENTS:
 *   - angle:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) rotate matrix.
 */
__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), s = sin(A), c = cos(A);
  MATR m = UnitMatrix;

  m.A[1][1] = c;
  m.A[1][2] = s;
  m.A[2][1] = -s;
  m.A[2][2] = c;

  return m;
} /* End of 'MatrRotateX' function */

/* Set rotate around Y-vector matrix for vector by angle function.
 * ARGUMENTS:
 *   - angle:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) rotate matrix.
 */
__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), s = sin(A), c = cos(A);
  MATR m = UnitMatrix;

  m.A[0][0] = c;
  m.A[0][2] = -s;
  m.A[2][0] = s;
  m.A[2][2] = c;

  return m;
} /* End of 'MatrRotateY' function */

/* Set rotate around Z-vector matrix for vector by angle function.
 * ARGUMENTS:
 *   - angle:
 *       FLT AngleInDegree;
 * RETURNS:
 *   (MATR) rotate matrix.
 */
__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), s = sin(A), c = cos(A);
  MATR m = UnitMatrix;

  m.A[0][0] = c;
  m.A[0][1] = s;
  m.A[1][0] = -s;
  m.A[1][1] = c;

  return m;
} /* End of 'MatrRotateZ' function */

/* Set rotate around vector matrix for vector by angle function.
 * ARGUMENTS:
 *   - rotate by vector:
 *       VEC R;
 *   - angle:
 *       FLT AngleInDegre;
 * RETURNS:
 *   (MATR) rotate matrix.
 */
__inline MATR MatrRotate( VEC R, FLT AngleInDegree )
{
  FLT A = D2R(AngleInDegree), s = sin(A), c = cos(A);
  VEC V = VecNormalize(R);
  MATR m =
  {
    {
      {c + V.X * V.X * (1 - c), V.X * V.Y * (1 - c) + V.Z * s, V.X * V.Z * (1 - c) + V.Z * s, 0},
      {V.Y * V.X * (1 - c) - V.Z * s, c + V.Y * V.Y * (1 - c), V.Y * V.Y * (1 - c) + V.X * s, 0},
      {V.Z * V.X * (1 - c) + V.Y * s, V.Z * V.Y * (1 - c) - V.X * s, c + V.Z * V.Z * (1 - c), 0},
      {0, 0, 0, 1}
    }
  };
  
  return m;
} /* End of 'MatrRotate' function */

/* Multiplicate two matrixes function.
 * ARGUMENTS:
 *   - matrixes to multiplicate:
 *       MATR M1, M2;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR r = {{{0}}};

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];

  return r;
} /* End of 'MatrMulMatr' function */

/* Get determinant of 3x3 matrix function.
 * ARGUMENTS:
 *   - elements of matrix:
 *       FLT A11, A12, A13, A21, A22, A23, A31, A32, A33;
 * RETURNS:
 *   (FLT) determinant.
 */
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

/* Get determinant of matrix function.
 * ARGUMENTS:
 *   - matrix:
 *       MATR M;
 * RETURNS:
 *   (FLT) determinant.
 */
__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

/* Get inverse matrix by original function.
 * ARGUMENTS:
 *   - matrix:
 *       MATR M;
 * RETURNS:
 *   (MATR) inversed matrix.
 */
__inline MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return MatrIdentity();

  /* build adjoint matrix */
  r.A[0][0] =
    +MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[0][1] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[0][2] =
    +MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[0][3] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[1][0] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[1][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[2][0] =
    +MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                    M.A[1][0], M.A[1][2], M.A[1][3],
                    M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[3][0] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[3][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                    M.A[1][0], M.A[1][2], M.A[1][3],
                    M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;
} /* End of 'MatrInverse' function */

/* Transpose matrix function.
 * ARGUMENTS:
 *   - matrix:
 *       MATR M;
 * RETURNS:
 *   (MATR) Transpossed matrix.
 */
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  FLT tmp = M.A[0][0];

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      if (i != j)
      {
          M.A[i][j] = tmp;
          M.A[i][j] = M.A[j][i];
          M.A[j][i] = tmp;
      }

  return M;
} /* End of 'MatrTranspose' function */

/* Multiply vector and matrix function.
 * ARGUMENTS:
 *   - matrix:
 *       MATR M;
 *   - vector:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];
 
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

/* Transform point by matrix function.
 * ARGUMENTS:
 *   - point:
 *       VEC V;
 *   - matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]),
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]),
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]));
} /* End of 'PointTransform' funvtion*/

/* Transform vector by matrix function.
 * ARGUMENTS:
 *   - vector:
 *       VEC V;
 *   - matrix:
 *       MATR M;
 * RETURNS:
 *   (MATR) inversed matrix.
 */
__inline VEC VecTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VecTransform' function */

/* Set view matrix function.
 * ARGUMENTS:
 *   - radius-vectors:
 *       VEC Loc, At;
 *   - free vector:
 *       VEC Up1;
 * RETURNS:
 *   (MATR) view matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecCrossVec(Right, Dir);
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Set matrix of orthographic projection function.
 * ARGUMENTS:
 *   - grinds of projection box:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) matrix of orthographic projection.
 */
__inline MATR MatrOrtho( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  if (Far == Near)
    return MatrIdentity();

  return MatrSet(2 / (Right - Left), 0, 0, 0,
                 0, 2 / (Top - Bottom), 0, 0,
                 0, 0, -2 / (Far - Near), 0,
                 -(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom),
                 -(Far + Near) / (Far - Near), 1);
} /* End of 'MatrOrtho' function*/

/* Set view frustum matrix function.
 * ARGUMENTS:
 *   - grinds of projection box:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) view frustum matrix projection.
 */
__inline MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  if (Right == Left || Top == Bottom || Far == Near)
    return MatrIdentity();

  return MatrSet(2 * Near / (Right - Left), 0, 0, 0,
                 0, 2 * Near / (Top - Bottom), 0, 0,
                 (Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),
                 -(Far + Near) / (Far - Near), -1,
                 0, 0, -2 * Far * Near / (Far - Near), 0);
} /* End of 'MatrFrustum' function*/

/***
 * Get random numbers functions
 ***/

/* Get random number from -1 to 1 function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (FLT) random number from -1 to 1.
 */
__inline FLT Rnd1( VOID )
{
  return 2.0 * rand() / (FLT)RAND_MAX - 1.0;
} /* End of 'Rnd1' function */

/* Get random number from 0 to 1 function.
 * ARGUMENTS: None;
 * RETURNS:
 *   (FLT) random number from 0 to 1.
 */
__inline FLT Rnd0( VOID )
{
  return rand() / (FLT)RAND_MAX;
} /* End of 'Rnd0' function */

#endif /*__mth_h_*/
/* END OF 'mth.h' FILE */
