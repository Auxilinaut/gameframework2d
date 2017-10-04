#include "player.h"
#include "simple_logger.h"

void initPlayer(Player *p, EntityManager *entMan)
{
	p->ent = &entMan->entList[0];
	p->ent = initEntity(p->ent);
	p->ent->update = &updatePlayer;
	p->ent->position = vector2d(400,400);
	p->ent->direction = 2;
}

void movePlayer(Entity *ent)
{

	/********
	* 7 6 5 *
	* 0   4 *
	* 1 2 3 *
	********/

	if (ent->direction == 1)
	{
		ent->velocity.x = -2;
		ent->velocity.y = 2;
	}
	else if (ent->direction == 2)
	{
		ent->velocity.x = 0;
		ent->velocity.y = 3;
	}
	else if (ent->direction == 3)
	{
		ent->velocity.x = 2;
		ent->velocity.y = 2;
	}
	else if (ent->direction == 0 || ent->direction == 4)
	{
		ent->velocity.x = 0;
		ent->velocity.y = 0;
	}
	else if (ent->direction == 5)
	{
		ent->velocity.x = 0;
		ent->velocity.y = -2;
		move(ent);
		ent->direction = 4;
	}
	else if (ent->direction == 7)
	{
		ent->velocity.x = 0;
		ent->velocity.y = -2;
		move(ent);
		ent->direction = 0;
	}
}

void updatePlayer(Entity *ent)
{
	movePlayer(ent);
	move(ent);
	if (ent->position.y > 800) ent->position.y = 800;
}