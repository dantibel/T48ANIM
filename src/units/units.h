/* FILE NAME: units.h
 * PURPOSE: 3D animation project.
 *          Animation units declaration.
 * PROGRAMMER: DB6
 * DATE: 18.02.2022
 */
#ifndef __units_h_
#define __units_h_

#include "def.h"
#include "anim/anim.h"

/* Unit car creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateCar( VOID );

/* Unit bounce ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateBBall( VOID );

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateCtrl( VOID );

/* Unit light creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateLight( VOID );

/* Unit grid creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateGrid( VOID );

/* Unit g3dm creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateG3DM( VOID );

/* Unit snow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSnow( VOID );

/* Unit sky creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSky( VOID );

/* Unit smoke creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSmoke( VOID );

/* Tessellated unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateTess( VOID );

/* Spline unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT * DB6_UnitCreateSpline( VOID );

/* Unit noise grid creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (db6UNIT *) pointer to created unit.
 */
db6UNIT* DB6_UnitCreateNoise( VOID );

#endif /* __units_h_ */
/* END OF 'units.h' FILE */
