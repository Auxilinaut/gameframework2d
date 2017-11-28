#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"
#include "animation.h"
#include "gf2d_draw.h"

typedef struct Entity_S {

	Sprite *sprite;
	float frames;
	float currFrame;
	TextLine name;
	Vector4D colorShift;

	struct AnimList_S *animList;
	TextLine currAnim;
	struct AnimReturnType_S *animRetType;  /**<set automatically each frame*/

	Bool active;
	Bool onScreen;
	Vector2D position;
	Vector2D velocity;
	SDL_Rect bounds;
	Vector4D boxColor;
	Bool jumping;
	Bool falling;
	int jumpTime; // [0, JUMP_HEIGHT]
	Bool colliding;
	Uint8 direction; // [0-6,255] 0 = left, increment counterclockwise

	Bool alive;
	float HP;
	float dmgTaken;
	float dmgDealt;

	struct Entity_S *parent;

	void(*update)(struct Entity_S *self);
	void(*draw)(struct Entity_S *self);
	void(*kill)(struct Entity_S *self);
	void(*free)(struct Entity_S *self, int *entRef);

}Entity;

/**
* @brief initialize entity variables
* @param ent reference to self
* @return pointer to initialized entity (still requires sprite, frames; todo: animations)
*/
Entity *initEntity(Entity *ent);

/**
* @brief update entity physics, collisions, HP, and damage
* @param ent reference to self
*/
void updateEntity(Entity *ent);

/**
* @brief handle animations and then draw entity
* @param ent reference to self
*/
void drawEntity(Entity *ent);

/**
* @brief kill the entity, but keep it in world
* @param ent reference to self
*/
void killEntity(Entity *ent);

/**
* @brief free space for next entity when this entity is removed from world
* @param ent reference to self
*/
void freeEntity(Entity *ent, int *entRef);

#endif // !__ENTITY_H_