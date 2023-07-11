/* FILE NAME: rndlight.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Uniform buffers support.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#include "anim/rnd/rnd.h"

/* Textures stock */
db6UBO DB6_RndUBOs[DB6_MAX_UBOS]; /* Array of UBO */
INT DB6_RndUBOsSize;              /* UBO array store size */

/* UBO handle initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndUBOInit( VOID )
{
  DB6_RndUBOsSize = 0;
} /* End of 'DB6_RndUBOInit' function */

/* UBO handle deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndUBOClose( VOID )
{
  INT i;

  for (i = 0; i < DB6_RndTexturesSize; i++)
    glDeleteBuffers(1, &DB6_RndUBOs[i].UBOId);
  DB6_RndUBOsSize = 0;
} /* End of 'DB6_RndUBOClose' function */

/* Add UBO to stock function.
 * ARGUMENTS:
 *   - UBO name:
 *       CHAR *Name;
 *   - OpenGL UBO bind point:
 *       UINT BindPoint;
 *   - UBO size in 16 bytes blocks:
 *       INT NumOfBlocks;
 *   - UBO data:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) UBO stock number (0 if error is occured).
 */
INT DB6_RndUBOCreate( CHAR *Name, UINT BindPoint, INT NumOfBlocks, VOID *Bits )
{
  if (DB6_RndUBOsSize >= DB6_MAX_UBOS)
    return -1;

  /* Setup OpenGL UBO */
  glGenBuffers(1, &DB6_RndUBOs[DB6_RndUBOsSize].UBOId);
  glBindBuffer(GL_UNIFORM_BUFFER, DB6_RndUBOs[DB6_RndUBOsSize].UBOId);
  glBufferData(GL_UNIFORM_BUFFER, NumOfBlocks * 16, NULL, GL_STATIC_DRAW);

  if (Bits != NULL)
    glBufferSubData(GL_UNIFORM_BUFFER, 0, NumOfBlocks * 16, Bits);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  /* Add to stock */
  DB6_RndUBOs[DB6_RndUBOsSize].Size = NumOfBlocks;
  DB6_RndUBOs[DB6_RndUBOsSize].BindPoint = BindPoint;
  strncpy(DB6_RndUBOs[DB6_RndUBOsSize].Name, Name, DB6_STR_MAX - 1);

  return DB6_RndUBOsSize++;
} /* End of 'DB6_RndUBOAdd' function */

/* UBO data update function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 *   - UBO offset 16 bytes blocks:
 *       INT BlocksOffset;
 *   - UBO size in 16 bytes blocks:
 *       INT NumOfBlocks;
 *   - UBO data:
 *       VOID *Bits;
 * RETURNS: None.
 */
VOID DB6_RndUBOUpdate( INT UBONo, INT BlocksOffset, INT NumOfBlocks, VOID *Bits )
{
  if (UBONo < 0 || UBONo >= DB6_RndUBOsSize)
    return;

  /* Update UBO data */
  if (BlocksOffset >= DB6_RndUBOs[UBONo].Size)
    return;
  if (BlocksOffset < 0)
    BlocksOffset = 0;
  if (NumOfBlocks == 0)
    NumOfBlocks = DB6_RndUBOs[UBONo].Size;
  if (BlocksOffset + NumOfBlocks >= DB6_RndUBOs[UBONo].Size)
    NumOfBlocks = DB6_RndUBOs[UBONo].Size - BlocksOffset;

  glBindBuffer(GL_UNIFORM_BUFFER, DB6_RndUBOs[UBONo].UBOId);
  glBufferSubData(GL_UNIFORM_BUFFER, BlocksOffset * 16, NumOfBlocks * 16, Bits);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
} /* End of 'DB6_RndUBOUpdate' function */

/* UBO apply function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 * RETURNS: None.
 */
VOID DB6_RndUBOApply( INT UBONo )
{
  if (UBONo < 0 || UBONo >= DB6_RndUBOsSize)
    return;

  glBindBufferBase(GL_UNIFORM_BUFFER, DB6_RndUBOs[UBONo].BindPoint, DB6_RndUBOs[UBONo].UBOId);
} /* End of 'DB6_RndUBOApply' function */

/* END OF 'rndlight.c' FILE */
