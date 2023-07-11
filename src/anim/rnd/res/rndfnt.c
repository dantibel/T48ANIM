/* FILE NAME: rndshd.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementation module.
 *          Resource implementation.
 *          Materials hadle module.
 * PROGRAMMER: DB6
 * DATE: 31.01.2022
 */
#include <stdio.h>

#include "anim/rnd/rnd.h"

/* Font data */
static db6FONT DB6_RndFntFont;

/* Font characters primitives */
static db6PRIM DB6_RndFntChars[256];

/* Font material */
static INT DB6_RndFntMtlNo;

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DB6_RndFntLoad( CHAR *FileName )
{
  FILE *F;
  INT i;
  DWORD Sign, W, H, *tex;
  struct
  {
    VEC P;  /* Char enpoint position */
    VEC2 T; /* Char enpoint texture coordinates */
    VEC N;  /* Char enpoint normal (not used) */
    VEC4 C; /* Char enpoint color (not used) */
  } Chars[256][4];
  db6MATERIAL mtl;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }
  fread(&DB6_RndFntFont, sizeof(db6FONT), 1, F);
  fread(Chars, sizeof(Chars), 1, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);

  if ((tex = malloc(W * H * 4)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(tex, 4, W * H, F);
  fclose(F);

  mtl = DB6_RndMtlGetDef();
  strncpy(mtl.Name, FileName, DB6_STR_MAX - 1);
  mtl.Tex[0] = DB6_RndTexAddImg(FileName, W, H, 4, tex);
  free(tex);
  mtl.ShdNo = DB6_RndShdAdd("font");
  DB6_RndFntMtlNo = DB6_RndMtlAdd(&mtl);

  for (i = 0; i < 256; i++)
  {
    INT k;
    db6VERTEX v[4];

    for (k = 0; k < 4; k++)
      v[k].P = Chars[i][k].P, v[k].T = Chars[i][k].T,
        v[k].N = VecSet(0, 0, 1), v[k].C = Vec4Set1(1);

    DB6_RndPrimFree(&DB6_RndFntChars[i]);
    DB6_RndPrimCreate(&DB6_RndFntChars[i], DB6_RND_PRIM_TRISTRIP, v, 4, NULL, 0); 
    DB6_RndFntChars[i].MtlNo = DB6_RndFntMtlNo;
  }

  return TRUE;
} /* End of 'DB6_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndFntInit( VOID )
{
  DB6_RndFntLoad("bin/fonts/consolas.g3df");
} /* End of 'DB6_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    DB6_RndPrimFree(&DB6_RndFntChars[i]);
  memset(DB6_RndFntChars, 0, sizeof(DB6_RndFntChars));
} /* End of 'DB6_RndFntInit' function */

/* Draw string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC Pos;
 *   - font size:
 *      FLT Size;
 * RETURNS: None.
 */
VOID DB6_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  VEC p = Pos;
 
 while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = p.X, Pos.Y -= Size;
    else
      if (DB6_RndFntFont.AdvanceX[(UCHAR)*Str] != 0)
      {
        DB6_RndPrimDraw(&DB6_RndFntChars[(UCHAR)*Str],
                        MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
        Pos.X += DB6_RndFntFont.AdvanceX[(UCHAR)*Str] * Size;
      }
    Str++;
  }
} /* End of 'DB6_RndFntDraw' function */
/* END OF 'rndfnt.c' FILE */
