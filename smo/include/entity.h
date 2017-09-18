#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"

typedef struct Entity_S {
	Sprite *sprite;
	float frames;
	float currFrame;
	Vector4D colorShift;
	Vector2D position;
	Vector2D velocity;
	Bool active;
	Bool alive;
	float HP;
	float dmgTaken;
	struct Entity_S *parent;
	void(*draw)(struct Entity_S *self);
	void(*update)(struct Entity_S *self);
	//void(*free)(struct Entity_S *self);
}Entity;

/**
* @brief initialize entity
* @param ent reference to self
* @param sprite reference to sprite
* @param parent reference to parent entity
* @param frames number of frames in entity animation
*/
void initEntity(Entity *ent, Sprite *sprite, Entity *parent, float frames);
void drawEntity(Entity *ent);
void updateEntity(Entity *ent);
void freeEntity(Entity *ent);

#endif // !__ENTITY_H_