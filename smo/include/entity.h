#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"

typedef struct Entity_S {
	Vector2D velocity;
	Vector2D position;
	float HP;
	float dmgTaken;
	Sprite entSprite;
	Bool inUse;
	struct Entity_S *parent;
	void(*init)(struct Entity_S *self);
	void(*draw)(struct Entity_S *self);
	void(*update)(struct Entity_S *self);
	void(*free)(struct Entity_S *self);
}Entity;

void initEntity(Entity *ent);
void drawEntity(Entity *ent);
void updateEntity(Entity *ent);
void freeEntity(Entity *ent);

#endif // !__ENTITY_H_