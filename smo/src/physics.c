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
	if (bottomA <= topB) return false;

	if (topA >= bottomB) return false;

	if (rightA <= leftB) return false;

	if (leftA >= rightB) return false;

	//If none of the sides from A are outside B
	return true;
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

void scrollUp(double *yPos, double spd, Entity *ent, int *entRef)
{

	double scrollTo = *yPos - spd;

	if (ent != NULL)
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