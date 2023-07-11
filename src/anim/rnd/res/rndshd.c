/* FILE NAME: rndshd.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Resource implementation.
 *          Shaders hadle module.
 * PROGRAMMER: DB6
 * DATE: 11.03.2022
 */
#include <stdio.h>

#include "anim/rnd/res/rndres.h"

/* Shader stock */
db6SHADER DB6_RndShaders[DB6_MAX_SHADERS]; /* Shaders array */
INT DB6_RndShadersSize;                    /* Shaders array size */

/* Addons for shaders programs */
INT DB6_RndShdAddonI[8];
FLT DB6_RndShdAddonF[8];

/* Load text from file function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName.
 * RETURNS:
 *   (CHAR *) Load text (allocated through malloc).
 */
static CHAR * DB6_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Get file name */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  rewind(F);
  fread(txt, 1, flen, F);
  txt[flen] = 0;

  fclose(F);
  return txt;
} /* End of 'DB6_RndShdLoadTextFromFile' function */

/* Shader program load function.
 * ARGUMENTS:
 *   - shader directory files prefix (based from /bin/shaders/):
 *       CHAR *FileNamePrefix;
 *   - shader handle part name:
 *       CHAR *Part;
 *   - logging text:
 *       CHAR *Text.
 * RETURNS: None.
 */
static VOID DB6_RndShdLog( CHAR *FileNamePrefix, CHAR *Part, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("bin/shaders/DB6_SHD.LOG", "a")) == NULL)
    return;

  fprintf(F, "%s/%s.glsl\n%s\n", FileNamePrefix, Part, Text);
  fclose(F);
} /* End of 'DB6_RndShdLog' function */

/* Global shader index value */
INT DB6_RndShdProgId;

/* Shader program load function.
 * ARGUMENTS:
 *   - shader directory files prefix (basd from /bin/shaders/):
 *       CHAR *FileNamePrefix.
 * RETURNS:
 *   (INT) shader program index (0 if error is occured)
 */
#if 0
INT DB6_RndShdLoad( CHAR *FileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader name */
    INT Type;   /* Shader type (see GL_***_SHADER) */
    INT Id;     /* Result shader Id */
  } shds[] =
  {
    {"vert", GL_VERTEX_SHADER}, {"frag", GL_FRAGMENT_SHADER}, {"geom", GL_GEOMETRY_SHADER},
    {"ctrl", GL_TESS_CONTROL_SHADER}, {"eval", GL_TESS_EVALUATION_SHADER}
  };
  INT res, prg, i, NumOfS = sizeof(shds) / sizeof(shds[0]);
  BOOL isok = TRUE;
  CHAR *com = "", *txt, inc[DB6_STR_MAX] = "#include <commondf.glsl>", *str;
  static CHAR Buf[10000];

  for (i = 0; i < NumOfS && isok; i++)
  {
    /* Load file */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shds[i].Name);
    str = DB6_RndShdLoadTextFromFile(Buf);
    if (str == NULL)
    {
      if (i >= 2)
        continue;
      DB6_RndShdLog(FileNamePrefix, shds[i].Name, "Error load file");
      isok = FALSE;
      break;
    }

    if (strstr(str, inc) != NULL)
      com = DB6_RndShdLoadTextFromFile("bin/shaders/commondf.glsl");
    if ((txt = malloc(strlen(com) + strlen(str) + 1)) != NULL)
    {
      txt = strcat(com, str);
      com = str = NULL;
    }

    /* Create shader */
    if ((shds[i].Id = glCreateShader(shds[i].Type)) == 0)
    {
      DB6_RndShdLog(FileNamePrefix, shds[i].Name, "Error shader creation");
      free(txt);
      isok = FALSE;
      break;
    }

    /* Attach shader text */
    glShaderSource(shds[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shds[i].Id);
    glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
      DB6_RndShdLog(FileNamePrefix, shds[i].Name, Buf);
      isok = FALSE;
      break;
    }
  }

  if (isok)
    if ((prg = glCreateProgram()) == 0)
    {
      DB6_RndShdLog(FileNamePrefix, "PROG", "Error program creation");
      isok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NumOfS; i++)
        if (shds[i].Id != 0)
          glAttachShader(prg, shds[i].Id);

      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        DB6_RndShdLog(FileNamePrefix, "PROG", Buf);
        isok = FALSE;
      }
    }

  if (!isok)
  {
    /* Delete shaders */
    for (i = 0; i < NumOfS; i++)
      if (shds[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shds[i].Id);
        glDeleteShader(shds[i].Id);
      }

    /* Delete program */
    if (prg != 0)
      glDeleteShader(prg);
    prg = 0;
  }

  return prg;
} /* End of 'DB6_RndShdLoad' function */
#endif
//#if 0
INT DB6_RndShdLoad( CHAR *FileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader name */
    INT Type;   /* Shader type (see GL_***_SHADER) */
    INT Id;     /* Result shader Id */
  } shds[] =
  {
    {"vert", GL_VERTEX_SHADER}, {"frag", GL_FRAGMENT_SHADER}, {"geom", GL_GEOMETRY_SHADER},
    {"ctrl", GL_TESS_CONTROL_SHADER}, {"eval", GL_TESS_EVALUATION_SHADER}
  };
  INT res, prg = 0, i, NumOfS = sizeof(shds) / sizeof(shds[0]);
  BOOL isok = TRUE;
  CHAR *txt;
  static CHAR Buf[10000];

  for (i = 0; i < NumOfS && isok; i++)
  {
    /* Load file */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shds[i].Name);
    txt = DB6_RndShdLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      if (i >= 2)
        continue;
      DB6_RndShdLog(FileNamePrefix, shds[i].Name, "Error load file");
      isok = FALSE;
      break;
    }

    /* Create shader */
    if ((shds[i].Id = glCreateShader(shds[i].Type)) == 0)
    {
      DB6_RndShdLog(FileNamePrefix, shds[i].Name, "Error shader creation");
      free(txt);
      isok = FALSE;
      break;
    }

    /* Attach shader text */
    glShaderSource(shds[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shds[i].Id);
    glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
      DB6_RndShdLog(FileNamePrefix, shds[i].Name, Buf);
      isok = FALSE;
      break;
    }
  }

  if (isok)
    if ((prg = glCreateProgram()) == 0)
    {
      DB6_RndShdLog(FileNamePrefix, "PROG", "Error program creation");
      isok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NumOfS; i++)
        if (shds[i].Id != 0)
          glAttachShader(prg, shds[i].Id);

      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        DB6_RndShdLog(FileNamePrefix, "PROG", Buf);
        isok = FALSE;
      }
    }

  if (!isok)
  {
    /* Delete shaders */
    for (i = 0; i < NumOfS; i++)
      if (shds[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shds[i].Id);
        glDeleteShader(shds[i].Id);
      }

    /* Delete program */
    if (prg != 0)
      glDeleteShader(prg);
    prg = 0;
  }

  return prg;
} /* End of 'DB6_RndShdLoad' function */
//#endif

/* Shader program free function.
 * ARGUMENTS:
 *   - shader program index:
 *       INT ShdNo.
 * RETURNS: None.
 */
VOID DB6_RndShdFree( INT ShdNo )
{
  INT shds[5] = {0}, n, i;

  if (ShdNo == 0 || !glIsProgram(ShdNo))
    return;

  glGetAttachedShaders(ShdNo, 5, &n, shds);

  /* Delete shaders */
  for (i = 0; i < n; i++)
    if (shds[i] != 0 && glIsShader(shds[i]))
    {
      glDetachShader(ShdNo, shds[i]);
      glDeleteShader(shds[i]);
    }

  /* Delete program */
  glDeleteProgram(ShdNo);
} /* End of 'DB6_RndShdFree' function */

/* Initialization of shaders stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndShdInit( VOID )
{
  DB6_RndShadersSize = 1;
  if ((DB6_RndShaders[0].ProgId = DB6_RndShdLoad("default")) != -1)
    strncpy(DB6_RndShaders[0].Name, "default", DB6_STR_MAX - 1);
} /* End of 'DB6_RndShdInit' function */

/* Deinitialization of program shader stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < DB6_RndShadersSize; i++)
  {
    DB6_RndDirWatcher_StopWatch(&DB6_RndShaders[i].Watcher);
    DB6_RndShdFree(DB6_RndShaders[i].ProgId);
  }
  DB6_RndShadersSize = 0;
} /* End of 'DB6_RndShdClose' function */

/* Add program shader to shaders stock function.
 * ARGUMENTS:
*   - shader directory files prefix (based from /bin/shaders/):
 *       CHAR *FileNamePrefix.
 * RETURNS: Shader stock index.
 */
INT DB6_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;
  CHAR Buf[_MAX_PATH];
  DB6_DIR_WATCHER DirWtch = {{0}};

  for (i = 0; i < DB6_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, DB6_RndShaders[i].Name) == 0)
      return i;

  if (DB6_RndShadersSize >= DB6_MAX_SHADERS)
    return 0;

  DB6_RndShaders[i].ProgId = DB6_RndShdLoad(ShaderFileNamePrefix);
  strncpy(DB6_RndShaders[i].Name, ShaderFileNamePrefix, DB6_STR_MAX - 1);

  sprintf(Buf, "./bin/shaders/%s", DB6_RndShaders[i].Name);
  //strncpy(DB6_RndShaders[i].Watcher.Path, Buf, _MAX_PATH);
  DB6_RndDirWatcher_StartWatch(&DB6_RndShaders[i].Watcher, Buf);

  return DB6_RndShadersSize++;
} /* End of 'DB6_RndShdAdd' function */

/* Update all program shaders in shaders stock function.
 * ARGUMENTS: None.
 * RETURNS: Shader stock index.
 */
VOID DB6_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < DB6_RndShadersSize; i++)
  {
    //if (DB6_RndDirWatcher_IsChanged(&DB6_RndShaders[i].Watcher))
    //{
      DB6_RndShdFree(DB6_RndShaders[i].ProgId);
      DB6_RndShaders[i].ProgId = DB6_RndShdLoad(DB6_RndShaders[i].Name);
    //}
  }
} /* End of 'DB6_RndShdUpdate' function */

/* Start directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       DB6_DIR_WATCHER *Wtch;
 *   - directory path:
 *       CHAR *DirPath;
 * RETURNS: None.
 */ 
VOID DB6_RndDirWatcher_StartWatch( DB6_DIR_WATCHER *Wtch, CHAR *DirPath )
{
  DB6_RndDirWatcher_StopWatch(Wtch);
  ZeroMemory(&Wtch->Ovr, sizeof(OVERLAPPED));
  strncpy(Wtch->Path, DirPath, _MAX_PATH);

  /* Open directory */
  Wtch->hDir =
    CreateFile(DirPath,
      FILE_LIST_DIRECTORY,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL, OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
      NULL);

  /* Start monitoring */
  Wtch->Ovr.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ReadDirectoryChangesW(Wtch->hDir,
    Wtch->Buffer, sizeof(Wtch->Buffer),
    TRUE,
    FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME,
    NULL,
    &Wtch->Ovr,
    NULL);
} /* End of 'DB6_RndDirWatcher_StartWatch' function */

/* Stop directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       DB6_DIR_WATCHER *Wtch;
 * RETURNS: None.
 */
VOID DB6_RndDirWatcher_StopWatch( DB6_DIR_WATCHER *Wtch )
{
  if (Wtch->Ovr.hEvent != NULL)
    CloseHandle(Wtch->Ovr.hEvent);
  if (Wtch->hDir != NULL)
    CloseHandle(Wtch->hDir);

  ZeroMemory(&Wtch->Ovr, sizeof(OVERLAPPED));
  Wtch->hDir = NULL;
  Wtch->Path[0] = 0;
} /* End of 'DB6_RndDirWatcher_StopWatch' function */

/* Check if directory is changed function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       DB6_DIR_WATCHER *Wtch;
 * RETURNS:
 *   (BOOL) TRUE if directory contents is changed (and subfolders).
 */
BOOL DB6_RndDirWatcher_IsChanged( DB6_DIR_WATCHER *Wtch )
{
  INT id;

  if (Wtch->Ovr.hEvent == NULL || Wtch->hDir == NULL)
    return FALSE;

  id = WaitForSingleObject(Wtch->Ovr.hEvent, 0);
  if (id == WAIT_OBJECT_0)
  {
    /* Restart monitoring */
    ResetEvent(Wtch->Ovr.hEvent);
    ReadDirectoryChangesW(Wtch->hDir,
      Wtch->Buffer, sizeof(Wtch->Buffer),
      TRUE,
      FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
        FILE_NOTIFY_CHANGE_FILE_NAME,
      NULL,
      &Wtch->Ovr,
      NULL);
    /* StartWatch(Wtch->Path); */
    return TRUE;
  }
  return FALSE;
} /* End of 'DB6_RndDirWatcher_IsChanged' function */

/* END OF 'rndshd.c' FILE */
