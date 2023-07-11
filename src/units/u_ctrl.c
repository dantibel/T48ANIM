/* FILE NAME: u_cam.c
 * PURPOSE: 3D animation project.
 *          Camera unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 14.03.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_CONTROL
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
} db6UNIT_CONTROL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_CONTROL *Uni, db6ANIM *Ani )
{
  DB6_RndCamSet(VecSet(0, 20, 10), VecSet(0, 15, 0), VecSet(0, 1, 0));
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_CONTROL *Uni, db6ANIM *Ani )
{
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_CONTROL *Uni, db6ANIM *Ani )
{
  FLT
    Dist = VecLen(VecSubVec(DB6_RndCamAt, DB6_RndCamLoc)),
    cosT = (DB6_RndCamLoc.Y - DB6_RndCamAt.Y) / Dist,
    sinT = sqrt(1 - cosT * cosT),
    plen = Dist * sinT,
    cosP = (DB6_RndCamLoc.Z - DB6_RndCamAt.Z) / plen,
    sinP = (DB6_RndCamLoc.X - DB6_RndCamAt.X) / plen,
    Azimuth = R2D(atan2(sinP, cosP)),
    Elevator = R2D(atan2(sinT, cosT));
  VEC dv;

  /* Keyboard response */
  if (Ani->Keys['1'])
    Azimuth++;
  if (Ani->Keys['2'])
    Elevator++;
  if (DB6_Anim.KeysClick[VK_SPACE]) /* Pause */
    DB6_Anim.IsPause = !DB6_Anim.IsPause;
  if (DB6_Anim.KeysClick[VK_ESCAPE]) /* Exit program */
    DB6_AnimClose();
  if (DB6_Anim.KeysClick[VK_F11]) /* Flip full screen */
    DB6_AnimFlipFullScreen();
  if (Ani->KeysClick['W'] || Ani->KeysClick['w']) /* switch wireframe mode */
  {
    if (Ani->IsWireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Ani->IsWireframe = !Ani->IsWireframe;
  }
  if (Ani->KeysClick['c'] || Ani->KeysClick['C'])
  {
    DB6_RndCamLoc = VecSet(0, 25, 20);
    DB6_RndCamAt = VecSet(0, 15, 0);
    DB6_RndCamUp = VecSet(0, 1, 0);

    Dist = VecLen(VecSubVec(DB6_RndCamAt, DB6_RndCamLoc));
    cosT = (DB6_RndCamLoc.Y - DB6_RndCamAt.Y) / Dist,
    sinT = sqrt(1 - cosT * cosT),
    cosP = (DB6_RndCamLoc.Z - DB6_RndCamAt.Z) / (Dist * sinT),
    sinP = (DB6_RndCamLoc.X - DB6_RndCamAt.X) / (Dist * sinT);

    Azimuth = R2D(atan2(sinP, cosP)),
    Elevator = R2D(atan2(sinT, cosT));
  }

  /* Handle camera orientation */
  Azimuth += Ani->GlobalDeltaTime * (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  if (Azimuth < 0.1)
    Azimuth = 0.1;
  if (Azimuth > 359.9)
    Azimuth = 359.9;

  Elevator += Ani->GlobalDeltaTime * (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));
  if (Elevator < 0.1)
    Elevator = 0.1;
  if (Elevator > 179.9)
    Elevator = 179.9;

  DB6_Debug("Azimuth: %g Elevator: %g\n", Azimuth, Elevator);

  Dist += Ani->GlobalDeltaTime * (-Ani->Mdz + 8 * (1 + Ani->Keys[VK_SHIFT] * 27) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));
  if (Dist < 0.1)
    Dist = 0.1;

  /* Handle camera position */
  if (Ani->Keys[VK_RBUTTON])
  {
    FLT sx, sy, Wp, Hp;

    Wp = Hp = DB6_RndProjSize;
    if (Ani->W > Ani->H)
      Wp *= (FLT)Ani->W / Ani->H;
    else
      Hp *= (FLT)Ani->H / Ani->W;

    sx = -Ani->Mdx * Wp / Ani->W * Dist / DB6_RndProjDist;
    sy = Ani->Mdy * Hp / Ani->H * Dist / DB6_RndProjDist;

    dv = VecAddVec(VecMulNum(DB6_RndCamRight, sx), VecMulNum(DB6_RndCamUp, sy));
    DB6_RndCamAt = VecAddVec(DB6_RndCamAt, dv);
    DB6_RndCamLoc = VecAddVec(DB6_RndCamLoc, dv);
  }

  /* Setup result camera */
  DB6_RndCamSet(PointTransform(VecSet(0, Dist, 0),
                               MatrMulMatr3(MatrRotateZ(Elevator), MatrRotateY(Azimuth), MatrTranslate(DB6_RndCamAt))),
                DB6_RndCamAt, DB6_RndCamUp);

} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT_CONTROL *Uni, db6ANIM *Ani )
{
  CHAR buf[DB6_STR_MAX];
  VEC save_cam_loc = DB6_RndCamLoc, save_cam_at = DB6_RndCamAt, save_cam_up = DB6_RndCamUp;
  MATR save_matr_proj = DB6_RndMatrProj;

  DB6_RndMatrProj = MatrOrtho(0, Ani->W - 1, -(Ani->H - 1), 0, -1, 1);
  DB6_RndMatrView = MatrIdentity();
  DB6_RndMatrVP = MatrMulMatr(DB6_RndMatrView, DB6_RndMatrProj);

  sprintf(buf, "FPS: %.0f", Ani->FPS);
  DB6_RndFntDraw(buf, VecSet(0, -(Ani->H - 60), 0), 32);
  sprintf(buf, "[ESC]-exit [SPACE]-pause [W]-switch wireframe [C]-reset camera");
  DB6_RndFntDraw(buf, VecSet(0, -(Ani->H - 30), 0), 28);

  DB6_RndMatrProj = save_matr_proj;
  DB6_RndMatrVP = MatrMulMatr(DB6_RndMatrView, DB6_RndMatrProj);
  DB6_RndCamSet(save_cam_loc, save_cam_at, save_cam_up);
} /* End of 'DB6_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateCtrl( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_CONTROL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateCtrl' function */

/* END OF 'u_cam.c' FILE */
