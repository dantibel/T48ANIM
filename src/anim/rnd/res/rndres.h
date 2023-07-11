/* FILE NAME: rndres.h
 * PURPOSE: 3D animation project.
 *          Rendering system module.
 *          Resource declaration.
 * PROGRAMMER: DB6
 * DATE: 31.01.2022
 */
#ifndef __rndres_h_
#define __rndres_h_

#define GLEW_STATIC
#include <glew.h>

#include <wglew.h>
#include <gl\wglext.h>

#pragma comment(lib, "opengl32")

#include "def.h"

#define DB6_STR_MAX 300

/***
 * Common resource handle functions
 ***/

/* Resource initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndResInit( VOID );

/* Resource closing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndResClose( VOID );

/***
 * Directory watcher functions
 ***/

/* Directory watcher handle structure */
typedef struct tagDB6_DIR_WATCHER
{
  OVERLAPPED Ovr;       /* Overlapped structure for changing hook */
  HANDLE hDir;          /* Traching directory handle */
  BYTE Buffer[27 * 47]; /* Read directory changes data buffer */
  CHAR Path[_MAX_PATH]; /* Store path */
} DB6_DIR_WATCHER;

/* Start directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       DB6_DIR_WATCHER *Wtch;
 *   - directory path:
 *       CHAR *DirPath;
 * RETURNS: None.
 */
VOID DB6_RndDirWatcher_StartWatch( DB6_DIR_WATCHER *Wtch, CHAR *DirPath );

/* Stop directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       DB6_DIR_WATCHER *Wtch;
 * RETURNS: None.
 */
VOID DB6_RndDirWatcher_StopWatch( DB6_DIR_WATCHER *Wtch );

/* Check if directory is changed function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       DB6_DIR_WATCHER *Wtch;
 * RETURNS:
 *   (BOOL) TRUE if directory contents is changed (and subfolders).
 */
BOOL DB6_RndDirWatcher_IsChanged( DB6_DIR_WATCHER *Wtch );

/***
 * Shaders block
 ***/

/* Shader program stock type */
typedef struct tagdb6SHADER
{
  CHAR Name[DB6_STR_MAX];  /* Shader filename prefix */
  INT ProgId;              /* Shader program Id */
  DB6_DIR_WATCHER Watcher; /* Shader dirictory watcher */
} db6SHADER;

/* Maximum size of shader stock */
#define DB6_MAX_SHADERS 30

/* Shader stock */
extern db6SHADER DB6_RndShaders[DB6_MAX_SHADERS]; /* Shaders array */
extern INT DB6_RndShadersSize;                    /* Shaders array size */

/* Global shader index value */
extern INT DB6_RndShdProgId;

/* Addons for shaders programs */
extern INT DB6_RndShdAddonI[8];
extern FLT DB6_RndShdAddonF[8];

/* Initialization of shader stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndShdInit( VOID );

/* Deinitialization of shader stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndShdClose( VOID );

/* Update all shaders program in shader stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndShdUpdate( VOID );

/* Add shader program to shader stock function.
 * ARGUMENTS:
*   - shader directory files prefix (based from /bin/shaders/):
 *       CHAR *FileNamePrefix.
 * RETURNS: Shader stock index.
 */
INT DB6_RndShdAdd( CHAR *ShaderFileNamePrefix );

/* Shader program free function.
 * ARGUMENTS:
 *   - shader program index:
 *       INT ShdNo;
 * RETURNS: None.
 */
VOID DB6_RndShdFree( INT ShdNo );

/* Shader program load function.
 * ARGUMENTS:
 *   - shader directory files prefix (basd from /bin/shaders/):
 *       CHAR *FileNamePrefix.
 * RETURNS:
 *   (INT) shader program index (0 if error is occured)
 */
INT DB6_RndShdLoad( CHAR *FileNamePrefix );

/***
 * Textures block
 ***/

/* Texture store type */
typedef struct tagdb6TEXTURE
{
  CHAR Name[DB6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */
} db6TEXTURE;

/* Maximum size of textures stock */
#define DB6_MAX_TEXTURES 300

/* Texture stock */
extern db6TEXTURE DB6_RndTextures[DB6_MAX_TEXTURES]; /* Textures array */
extern INT DB6_RndTexturesSize;                      /* Textures array size */

/* Initialization of texture stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndTexInit( VOID );

/* Deinitialization of texture stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndTexClose( VOID );

/* Add texture to texture stock function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size:
 *       INT W, H;
 *   - image array:
 *       VOID *ImageData.
 * RETURNS:
 *   (INT) textureindex (0 if error is occured)
 */
INT DB6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *ImageData );

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
INT DB6_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType, INT C, VOID *ImageData );

/* Add texture to texture stock from file function.
 * ARGUMENTS:
*   - shader directory files prefix (based from /bin/shaders/):
 *       CHAR *FileNamePrefix.
 * RETURNS: Texture stock index.
 */
INT DB6_RndTexAddFromFile( CHAR *FileName );

/***
 * Materials block
 ***/

/* Material type */
typedef struct tagdb6MATERIAL
{
  CHAR Name[DB6_STR_MAX]; /* Material name */

  /* Illumination coefficients */
  VEC Ka, Kd, Ks; /* Ambient, deffuse, specular coefficients */
  FLT Ph;         /* Phong power coefficient */

  FLT Trans;      /* Transparency factor */

  INT Tex[8];     /* Texture references from texture table (or -1) */

  INT ShdNo;      /* Shader number in shader table */
} db6MATERIAL;

/* Maximum size of materials stock */
#define DB6_MAX_MATERIALS 300

/* Material stock */
extern db6MATERIAL DB6_RndMaterials[DB6_MAX_MATERIALS]; /* Materials array */
extern INT DB6_RndMaterialsSize;                        /* Materials array size */

/* Initialization of material stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndMtlInit( VOID );

/* Deinitialization of material stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndMtlClose( VOID );

/* Add material to material stock function.
 * ARGUMENTS:
 *   - material data:
 *      db6MATERIAL *Mtl.
 * RETURNS:
 *   (INT) Material stock index.
 */
INT DB6_RndMtlAdd( db6MATERIAL *Mlt );

/* Get material from material stock function.
 * ARGUMENTS:
*   - material index:
 *       INT MtlNo.
 * RETURNS: Material stock index.
 */
db6MATERIAL * DB6_RndMtlGet( INT MtlNo );

/* Get default material function 
 * ARGUMENTS: None.
 * RETURNS: 
 *   (db6MATERIAL) default material;
 */
db6MATERIAL DB6_RndMtlGetDef( VOID );

/* function
 * ARGUMENTS:
 *   - Number of material in table of materials:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) ;
 */
UINT DB6_RndMtlApply( INT MtlNo );

/* Material free function.
 * ARGUMENTS:
 *   - materials index:
 *       INT MtlNo;
 * RETURNS: None.
 */
VOID DB6_RndMtlFree( INT MtlNo );

/***
 * Font block
 ***/

/* Font representation type */
typedef struct tgdb6FONT
{
  DWORD LineH, BaseH; /* Font line height and base line height in pixels */
  FLT AdvanceX[256];  /* Evere letter shift right value (0 if no letter present) */
} db6FONT;

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DB6_RndFntLoad( CHAR *FileName );

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndFntInit( VOID );

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndFntClose( VOID );

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
VOID DB6_RndFntDraw( CHAR *Str, VEC Pos, FLT Size );

/***
 * UBOs functions
 ***/

/* UBO representation type */
typedef struct tagdb6UBO
{
  CHAR Name[DB6_STR_MAX]; /* UBO name */
  INT Size;               /* UBO number of 16 bytes blocks */
  UINT UBOId;             /* OpenGL UBO Id */ 
  UINT BindPoint;         /* OpenGL UBO bind point */
} db6UBO;

/* UBOs stock */
#define DB6_MAX_UBOS 30
extern db6UBO DB6_RndUBOs[DB6_MAX_UBOS]; /* Array of UBO */
extern INT DB6_RndUBOsSize;              /* UBO array store size */

/* UBO handle initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndUBOInit( VOID );

/* UBO handle deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DB6_RndUBOClose( VOID );

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
INT DB6_RndUBOCreate( CHAR *Name, UINT BindPoint, INT NumOfBlocks, VOID *Bits );

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
VOID DB6_RndUBOUpdate( INT UBONo, INT BlocksOffset, INT NumOfBlocks, VOID *Bits );

/* UBO apply function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 * RETURNS: None.
 */
VOID DB6_RndUBOApply( INT UBONo );

#endif /* __rndres_h_ */
/* END OF 'rndres.h' FILE */
