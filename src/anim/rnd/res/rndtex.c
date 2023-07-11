/* FILE NAME: rndshd.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementation module.
 *          Resource implementation.
 *          Textures hadle module.
 * PROGRAMMER: DB6
 * DATE: 09.02.2022
 */
#include "anim/rnd/res/rndres.h"

/* Texture stock */
db6TEXTURE DB6_RndTextures[DB6_MAX_TEXTURES]; /* Textures array */
INT DB6_RndTexturesSize;                      /* Textures array size */

/* Initialization of texture stock function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTexInit( VOID )
{
  DB6_RndTexturesSize = 0;
} /* End of 'DB6_RndTexInit' function */

/* Deinitialization of texture stock function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndTexClose( VOID )
{
  INT i;
  
  for (i = 0; i < DB6_RndTexturesSize; i++)
    glDeleteTextures(1, &DB6_RndTextures[i].TexId);

  memset(DB6_RndShaders, 0, sizeof(db6TEXTURE) * DB6_RndTexturesSize);
  DB6_RndTexturesSize = 0;
} /* End of 'DB6_RndTexClose' function */

/* Add texture to texture stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size:
 *       INT W, H;
 *   - image array:
 *       VOID *ImageData;
 * RETURNS:
 *   (INT) textureindex (0 if error is occured)
 */
INT DB6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *ImageData )
{
  INT mips;

  if (DB6_RndTexturesSize >= DB6_MAX_TEXTURES)
    return -1;

  /* Setup OpenGl texture */
  glGenTextures(1, &DB6_RndTextures[DB6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, DB6_RndTextures[DB6_RndTexturesSize].TexId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 4 ? GL_RGBA8 : C == 3 ? GL_RGB8 : GL_R8, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
    C == 4 ? GL_BGRA : C == 3 ? GL_BGR : GL_LUMINANCE, GL_UNSIGNED_BYTE, ImageData);

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  /* Add to stock */
  strncpy(DB6_RndTextures[DB6_RndTexturesSize].Name, Name, DB6_STR_MAX);
  DB6_RndTextures[DB6_RndTexturesSize].W = W;
  DB6_RndTextures[DB6_RndTexturesSize].H = H;

  return DB6_RndTexturesSize++;
} /* End of 'DB6_RndTexAddImg' function */

/* Add texture to texture by format stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size:
 *       INT W, H;
 *   - OpenGL texture type:
 *       INT GLType;
 *   - bytes per pixel:
 *       INT C;
 *   - image array:
 *       VOID *ImageData;
 * RETURNS:
 *   (INT) textureindex (0 if error is occured)
 */
INT DB6_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType, INT C, VOID *ImageData )
{
  INT mips;

  if (DB6_RndTexturesSize >= DB6_MAX_TEXTURES)
    return -1;

  /* Setup OpenGl texture */
  glGenTextures(1, &DB6_RndTextures[DB6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, DB6_RndTextures[DB6_RndTexturesSize].TexId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;

  glTexStorage2D(GL_TEXTURE_2D, mips, GLType, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H,
    C == 4 ? GL_BGRA : C == 3 ? GL_BGR : GL_LUMINANCE, GL_UNSIGNED_BYTE, ImageData);

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  /* Add to stock */
  strncpy(DB6_RndTextures[DB6_RndTexturesSize].Name, Name, DB6_STR_MAX);
  DB6_RndTextures[DB6_RndTexturesSize].W = W;
  DB6_RndTextures[DB6_RndTexturesSize].H = H;

  return DB6_RndTexturesSize++;
} /* End of 'DB6_RndTexAddFmt' function */

/* Add texture to texture stock from file function.
 * ARGUMENTS:
 *   - shader directory files prefix (based from /bin/shaders/):
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) Texture stock index.
 */
INT DB6_RndTexAddFromFile( CHAR *FileName )
{
  INT res = -1;
  HBITMAP hBm;

  /* Load *.BMP file */
  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP bm;

    GetObject(hBm, sizeof(bm), &bm);
    res = DB6_RndTexAddImg(FileName, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel / 8, bm.bmBits);
    DeleteObject(hBm);
  }
  else
  {
    /* Load *.G24/*.G32 file */
    INT C = 0;
    FILE *F;

    if ((F = fopen(FileName, "rb")) != NULL)
    {
      INT W = 0, H = 0, flen;
      BYTE *mem;

      /* Read image size */
      fread(&W, 2, 1, F);
      fread(&H, 2, 1, F);
      fseek(F, 0, SEEK_END);
      flen = ftell(F);
      rewind(F);

      /* Calculate bytes per pixel count */
      if (flen == 4 + W * H * 3)
        C = 3;
      else if (flen == 4 + W * H * 4)
        C = 4;
      else if (flen == 4 + W * H)
        C = 1;
      if (C != 0 && (mem = malloc(W * H * C)) != NULL)
      {
        fread(mem, C, W * H, F);
        res = DB6_RndTexAddImg(FileName, W, H, C, mem);
        free(mem);
      }
      fclose(F);
    }
  }
  return res;
} /* End of 'DB6_RndTexAddFromFile' function */

/* END OF 'rndtex.c' FILE */
