/* FILE NAME: u_car.c
 * PURPOSE: 3D animation project.
 *          Car unit's methods implementation module.
 * PROGRAMMER: DB6
 * DATE: 21.01.2022
 */
#include "units.h"

typedef struct tagdb6UNIT_CAR
{
  DB6_UNIT_BASE_FIELDS; /* Basic unit methods */
  db6PRIM Car;      /* Car primitive */
  VEC Pos;          /* Car position */
  INT Index;        /* Car index */
} db6UNIT_CAR;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitInit( db6UNIT_CAR *Uni, db6ANIM *Ani )
{
  static CallNum = 0;

  switch (Uni->Index = CallNum++)
  {
  case 0:
    DB6_RndPrimLoad(&Uni->Car, "bin/objs/vano.obj");
    Uni->Pos = VecSet(0, 0, 0);
    Uni->Car.Trans = MatrScale(VecSet1(0.8));
    break;
  /*
  case 1:
    DB6_RndPrimLoad(&Uni->Car, "bin/objs/porche.obj");
    Uni->Pos = VecSet(0, 0, -5);
    Uni->Car.Trans = MatrRotateY(90);
    break;
  */
  }
  Uni->Car.MtlNo = 0;
  Uni->Car.Trans = MatrMulMatr3(Uni->Car.Trans, MatrScale(VecSet1(2)), MatrTranslate(Uni->Pos));
} /* End of 'DB6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitClose( db6UNIT_CAR *Uni, db6ANIM *Ani )
{
  DB6_RndPrimFree(&Uni->Car);
} /* End of 'DB6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitResponse( db6UNIT_CAR *Uni, db6ANIM *Ani )
{
  if (Ani->IsPause)
    return;
  //Uni->Car.Light = VecSubVec(Uni->Pos, Ani->Light);
  //Uni->Car.Trans = MatrMulMatr(Uni->Car.Trans, MatrRotateY(-.02));
} /* End of 'DB6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       db6UNIT *Uni;
 *   - animation context:
 *       db6ANIM *Ani;
 * RETURNS: None.
 */
static VOID DB6_UnitRender( db6UNIT_CAR *Uni, db6ANIM *Ani )
{
  DB6_RndPrimDraw(&Uni->Car, Uni->Car.Trans);
} /* End of 'DB6_UnitRender' function */

/* Unit car creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateCar( VOID )
{
  db6UNIT *Uni;

  if ((Uni = DB6_AnimUnitCreate(sizeof(db6UNIT_CAR))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DB6_UnitInit;
  Uni->Response = (VOID *)DB6_UnitResponse;
  Uni->Render = (VOID *)DB6_UnitRender;
  Uni->Close = (VOID *)DB6_UnitClose;

  return Uni;
} /* End of 'DB6_UnitCreateCar' function */

/* END OF 'u_car.c' FILE */
