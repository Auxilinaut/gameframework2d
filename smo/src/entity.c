#include "entity.h"
#include "simple_logger.h"

void initEntity(Entity *ent, Sprite *sprite, Entity *parent, float frames)
{
	ent->sprite = sprite;
	ent->frames = frames;
	ent->currFrame = 0;
	ent->draw = &drawEntity;
	ent->update = &updateEntity;
	//ent->free = &freeEntity;
}

void updateEntity(Entity *ent)
{
	//physics
	if (ent->velocity.x)
	{

	}
	else if (ent->velocity.y)
	{

	}

	//damage
	if (ent->dmgTaken)
	{
		ent->HP = ent->HP - ent->dmgTaken;
		ent->dmgTaken = 0;
	}
	
}

void drawEntity(Entity *ent)
{
	//update anim frame
	ent->currFrame += 0.1;
	if (ent->currFrame >= ent->frames)
	{
		ent->currFrame = 0;
	}

	gf2d_sprite_draw(
		ent->sprite,
		ent->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)(ent->currFrame));
}

void freeEntity(Entity *ent)
{

}