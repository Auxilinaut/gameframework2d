#include "skateboard.h"
#include "simple_logger.h"

void initSkateboard(Skateboard *sb, EntityManager *entMan)
{
	sb->ent = &entMan->entList[1]; //player id 0, skateboard id 1
	sb->ent = initEntity(sb->ent);
	sb->ent->update = &updateSkateboard;
	sb->ent->position = vector2d(400, 400);
	sb->ent->direction = 2;
	sb->ent->parent = &entMan->entList[0]; //player entity is parent
	sb->ent->onScreen = 1;
}

void moveSkateboard(Entity *ent)
{
	/**********
	*255 6  5 *
	* 0     4 *
	* 1  2  3 *
	**********/

	Uint8 plrDir = ent->parent->direction;
	Uint8 trick = 0;

	if (!ent->parent->jumping) //player not doing a trick
	{
		if (plrDir == 2)
		{
			if (strcmp(ent->currAnim, "down") != 0) setEntityAnim(ent, "down");
		}
		else if (plrDir == 0 || plrDir == 4)
		{
			if (strcmp(ent->currAnim, "side") != 0) setEntityAnim(ent, "side");
		}
		else if (plrDir == 1)
		{
			if (strcmp(ent->currAnim, "downleft") != 0) setEntityAnim(ent, "downleft");
		}
		else if (plrDir == 3)
		{
			if (strcmp(ent->currAnim, "downright") != 0) setEntityAnim(ent, "downright");
		}
		else if (plrDir == 4)
		{
			if (strcmp(ent->currAnim, "right") != 0) setEntityAnim(ent, "right");
		}
		else if (plrDir == 5)
		{
			if (strcmp(ent->currAnim, "upright") != 0) setEntityAnim(ent, "upright");
		}
		else  if (plrDir == 255)
		{
			if (strcmp(ent->currAnim, "upleft") != 0) setEntityAnim(ent, "upleft");
		}
	}
	else //player is doing a trick
	{
		if (ent->parent->jumpTime > JUMP_HEIGHT - JUMP_SPEED) trick = rand() % 3 + 1; 

		if (trick == 1)
		{
			if (strcmp(ent->currAnim, "shoveit") != 0) setEntityAnim(ent, "shoveit");
		}
		else if (trick == 2)
		{
			if (strcmp(ent->currAnim, "treflip") != 0) setEntityAnim(ent, "treflip");
		}
		else if (trick == 3)
		{
			if (strcmp(ent->currAnim, "kickflip") != 0) setEntityAnim(ent, "kickflip");
		}
	}

	//stick to player
	ent->position.x = ent->parent->position.x;
	ent->position.y = ent->parent->position.y + ent->parent->sprite->frame_h - 8;
}

void updateSkateboard(Entity *ent)
{
	moveSkateboard(ent);
	nextEntFrame(ent);
}