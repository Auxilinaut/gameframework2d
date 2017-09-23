#include "entity.h"
#include "simple_logger.h"

void initEntity(Entity *ent)
{
	ent->update = &updateEntity;
	ent->draw = &drawEntity;
	ent->kill = &killEntity;
	ent->free = &freeEntity;
}

void updateEntity(Entity *ent)
{
	//physics
	//move(ent);

	if (ent->colliding)
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
	//update anim frame if possible
	if (ent->frames)
	{
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
			(int)(ent->currFrame)
		);
	}
	else
	{
		gf2d_sprite_draw_image(ent->sprite, ent->position);
	}
	
}

void killEntity(Entity *ent)
{
	//tba
}

void freeEntity(Entity *ent, int *entRef)
{
	ent->active = 0;
	*entRef = *entRef - 1;
}