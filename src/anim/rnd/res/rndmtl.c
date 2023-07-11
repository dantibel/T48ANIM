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

/* Material stock */
db6MATERIAL DB6_RndMaterials[DB6_MAX_MATERIALS]; /* Materials array */
INT DB6_RndMaterialsSize;                        /* Materials array size */

/* Material program load function.
 * ARGUMENTS:
 *   - shader directory files prefix (basd from /bin/shaders/):
 *       CHAR *FileNamePrefix.
 * RETURNS:
 *   (INT) material index (0 if error is occured)
 */
INT DB6_RndMtlLoad( CHAR *FileNamePrefix )
{
  return 0;
} /* End of 'DB6_RndMtlLoad' function */

/* Material program free function.
 * ARGUMENTS:
 *   - material index:
 *       INT MtlNo.
 * RETURNS: None.
 */
VOID DB6_RndMtlFree( INT Prg )
{

} /* End of 'DB6_RndMtlFree' function */

/* Initialization of material stock function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndMtlInit( VOID )
{
  db6MATERIAL defmtl = DB6_RndMtlGetDef();

  DB6_RndMaterialsSize = 0;

  DB6_RndMtlAdd(&defmtl);
} /* End of 'DB6_RndMtlInit' function */

/* Deinitialization of material stock function.
 * ARGUMENTS: None;
 * RETURNS: None.
 */
VOID DB6_RndMtlClose( VOID )
{
  DB6_RndMaterialsSize = 0;
} /* End of 'DB6_RndMtlClose' function */

/* Add material to material stock function.
 * ARGUMENTS:
 *   - material data:
 *      db6MATERIAL *Mtl.
 * RETURNS:
*    (INT) Material stock index.
 */
INT DB6_RndMtlAdd( db6MATERIAL *Mtl )
{
  if (DB6_RndMaterialsSize >= DB6_MAX_MATERIALS)
    return -1;

  DB6_RndMaterials[DB6_RndMaterialsSize] = *Mtl;
  return DB6_RndMaterialsSize++;
} /* End of 'DB6_RndMtlAdd' function */

/* Get material from material stock function.
 * ARGUMENTS:
*   - material index:
 *       INT MtlNo.
 * RETURNS: Material stock index.
 */
db6MATERIAL * DB6_RndMtlGet( INT MtlNo )
{
  if (MtlNo < 0 || MtlNo > DB6_RndMaterialsSize)
    return NULL;
  return &DB6_RndMaterials[MtlNo];
} /* End of 'DB6_RndMtlGet' function */

/* Get default material function
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6MATERIAL) default material;
 */
db6MATERIAL DB6_RndMtlGetDef( VOID )
{
  INT i;
  db6MATERIAL defmtl;

  memset(&defmtl, 0, sizeof(defmtl));
  strncpy(defmtl.Name, "default", DB6_STR_MAX - 1);
  defmtl.Ka = VecSet1(0.1);
  defmtl.Kd = VecSet1(0.9);
  defmtl.Ks = VecSet1(0.3);
  defmtl.Ph = 30;
  defmtl.Trans = 1;
  for (i = 0; i < 8; i++)
    defmtl.Tex[i] = -1;
  defmtl.ShdNo = 0;

  return defmtl;
} /* End of 'DB6_RndMtlGetDef' function */

/* function
 * ARGUMENTS:
 *   - Number of material in table of materials:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) ;
 */
UINT DB6_RndMtlApply( INT MtlNo )
{
  INT loc, prg, i;
  db6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DB6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DB6_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= DB6_RndShadersSize)
    prg = 0;
  //DB6_Debug("Shader: %s\n", DB6_RndShaders[prg].Name);
  prg = DB6_RndShaders[prg].ProgId;

  glUseProgram(prg);

  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";

    tname[9] = i + '0';
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, DB6_RndTextures[mtl->Tex[i]].TexId);
      if ((loc = glGetUniformLocation(prg, "Tex")) != -1)
        glUniform1i(loc, i);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }

  return prg;
}
/* END OF 'rndmtl.c' FILE */
