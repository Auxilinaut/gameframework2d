#include "entity.h"
#include "simple_logger.h"

Entity *initEntity(Entity *ent)
{
	ent->active = 1;
	ent->alive = 1;
	ent->onScreen = 1;
	ent->spawnTimer = SDL_GetTicks();
	ent->direction = 2;
	ent->colliding = 0;
	ent->jumping = 0;
	ent->jumpTime = 0;
	ent->falling = 0;
	//ent->boxColor = vector4d(0,0,0,0);
	ent->update = &updateEntity;
	ent->draw = &drawEntity;
	ent->kill = &killEntity;
	ent->free = &freeEntity;
	ent->touch = &touchEntity;
	return ent;
}

void updateEntity(Entity *ent)
{
	if (ent->alive)
	{
		//physics
		if (ent->colliding)
		{
			//ent->boxColor = vector4d(255, 0, 0, 255);
		}
		else
		{
			//ent->boxColor = vector4d(0, 255, 0, 255);
		}

		ent->colliding = false;

		nextEntFrame(ent);

		//damage
		if (ent->dmgTaken)
		{
			ent->HP = ent->HP - ent->dmgTaken;
			ent->dmgTaken = 0;
		}
	}
	else if (ent->spawnTimer < SDL_GetTicks())
	{
		ent->position.x = rand() % SCREEN_WIDTH;
		ent->position.y = SCREEN_HEIGHT;
		ent->alive = 1;
	}
	
}

void drawEntity(Entity *ent)
{
	if (ent->alive)
	{
		if (ent->frames)
		{
			//use anim frame if possible
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

		if (ent->boxColor.w)
		{
			gf2d_draw_rect(ent->bounds, ent->boxColor);
		}
	}
}

void killEntity(Entity *ent)
{
	//play anim
	ent->alive = 0;
	ent->spawnTimer = SDL_GetTicks() + 3000;
}

void freeEntity(Entity *ent, int *entRef)
{
	//ent->active = 0;
	memset(ent, 0, sizeof(Entity));
	if (*entRef) *entRef = *entRef - 1;
}

void touchEntity(Entity *self, Entity *other)
{
	self->kill(self);
	other->kill(other);
}