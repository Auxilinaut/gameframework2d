#include "entity.h"
#include "simple_logger.h"

Entity *initEntity(Entity *ent)
{
	ent->active = 1;
	ent->direction = 2;
	ent->update = &updateEntity;
	ent->draw = &drawEntity;
	ent->kill = &killEntity;
	ent->free = &freeEntity;
	return ent;
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
		/*ent->currFrame += 0.25;
		if (ent->currFrame >= ent->frames)
		{
			ent->currFrame = 0;
		}*/

		(AnimReturnType)(ent->animRetType) = findNextFrame(ent->animList, &ent->currFrame, ent->currAnim);

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
	//ent->active = 0;
	memset(ent, 0, sizeof(Entity));
	if (*entRef) *entRef = *entRef - 1;
}