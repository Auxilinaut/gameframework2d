#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "entity.h"

/**
* @brief checks for collision between two bounds (rectangles)
* @param a rectangle A
* @param b rectangle B
* @return whether or not the bounds have collided
*/
Bool checkCollision(SDL_Rect a, SDL_Rect b);

/**
* @brief move an entity according to velocity
* @param ent the entity being moved
*/
void move( Entity *ent );

/**
* @brief turn an entity clockwise or counterclockwise
* @param dir pointer to direction entity is facing (0-7, 0=left, increment counterclockwise)
* @param clockwise whether or not entity is turning clockwise
*/
void turn(Uint8 *dir, Bool clockwise);

/**
* @brief moves entity or sprite position upward according to speed;
* also frees out-of-bounds entities or wraps background
* @param yPos the y position of what is being scrolled up
* @param spd the speed to scroll at
* @param ent (optional) the entity being scrolled
* @param entRef (optional) pointer to entity reference counter
*/
void scrollUp(double *yPos, Uint8 spd, Entity *ent, int *entRef);

#endif // !__PHYSICS_H__