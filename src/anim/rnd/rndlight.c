/* FILE NAME: rndlight.c
 * PURPOSE: 3D animation project.
 *          Rendering system implementtion module.
 *          Lighting support.
 * PROGRAMMER: DB6
 * DATE: 07.03.2022
 */
#include "anim/anim.h"

/* Shadow mapsize */
#define DB6_RND_SHADOW_MAP_SIZE 256

/* Shadow frame buffer identifer */
INT DB6_RndShadowFBO;

/* Shadow texture identifer */
INT DB6_RndShadowTexId;

MATR DB6_RndShadowMatr;

BOOL DB6_RndShadowPassFlag;

/* END OF 'rndlight.c' FILE */
