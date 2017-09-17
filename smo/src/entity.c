#include "entity.h"

Entity initEntity(Entity *ent)
{
	ent->draw(ent);

}

void updateEntity(Entity *ent)
{
	//physics


	//damage
	ent->HP = ent->HP - ent->dmgTaken;
	ent->dmgTaken = 0;

	
}

void drawEntity(Entity *ent)
{

}

void freeEntity(Entity *ent)
{

}