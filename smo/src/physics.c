#include "physics.h"
#include "simple_logger.h"

Bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB) return 0;

	if (topA >= bottomB) return 0;

	if (rightA <= leftB) return 0;

	if (leftA >= rightB) return 0;

	//If none of the sides from A are outside B
	return 1;
}

void gravity(Entity *ent)
{
	if (ent->jumping)
	{
		if (!ent->falling)
		{
			ent->jumpTime += JUMP_SPEED;
			ent->position.y -= JUMP_SPEED;
			if (ent->jumpTime >= JUMP_HEIGHT) ent->falling = 1;
		}
		ent->jumpTime -= GRAVITY;
		ent->position.y += GRAVITY;

		if (ent->jumpTime <= 0) ent->jumping = 0;
	}
}

void move( Entity *ent )
{
	if (ent->velocity.x || ent->velocity.y) vector2d_add(ent->position, ent->velocity, ent->position);
}

void turn( Uint8 *dir, Bool counterclockwise )
{

	/**********
	*255 6  5 *
	* 0     4 *
	* 1  2  3 *
	**********/

	if (counterclockwise)
	{
		*dir = (*dir + 1) % 8;
	}
	else
	{
		*dir = (*dir - 1) % 8;
	}
}

void jump(Entity *ent)
{
	if (!ent->jumping)
	{
		ent->jumping = 1;
		ent->falling = 0;
		ent->jumpTime = 0;
	}
}

void scrollUp( double *yPos, double spd, Entity *ent, int *entRef )
{

	double scrollTo = *yPos - spd;

	if (ent != NULL && ent->active)
	{
		if ((scrollTo + ent->sprite->frame_h) < 0)
		{
			ent->free(ent, entRef);
		}
	}
	else
	{
		if ((scrollTo + SCREEN_HEIGHT) < 0) scrollTo = SCREEN_HEIGHT;
	}

	*yPos = scrollTo;
}