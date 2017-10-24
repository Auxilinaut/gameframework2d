#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "entity.h"

#define GRAVITY 2
#define JUMP_HEIGHT 68
#define JUMP_SPEED 4

/**
* @brief checks for collision between two bounds (rectangles)
* @param a rectangle A
* @param b rectangle B
* @return whether or not the bounds have collided
*/
Bool checkCollision(SDL_Rect a, SDL_Rect b);

/**
* @brief used after an entity jumps
* @param ent the entity jumping
*/
void gravity(Entity *ent);

/**
* @brief move an entity according to velocity
* @param ent the entity being moved
*/
void move(Entity *ent);

/**
* @brief turn an entity clockwise or counterclockwise
* @param dir pointer to direction entity is facing
* @param counterclockwise whether or not entity is turning counterclockwise
*/
void turn(Uint8 *dir, Bool counterclockwise);

/**
* @brief make an entity jump
* @param ent the entity jumping
*/
void jump(Entity *ent);

/**
* @brief moves entity or sprite position upward according to speed;
* also frees out-of-bounds entities or wraps background
* @param yPos the y position of what is being scrolled up
* @param spd the speed to scroll at
* @param ent (optional) the entity being scrolled
* @param entRef (optional) pointer to entity reference counter
*/
void scrollUp(double *yPos, Sprite *spr, double spd, Entity *ent, int *entRef);

/**
* @brief sets the collision bounds of an entity
* @param ent pointer to entity
*/
void setBounds(Entity *ent);

#endif // !__PHYSICS_H__