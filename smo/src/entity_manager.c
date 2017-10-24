#include "entity_manager.h"
#include "player.h"

/*MANAGER*/

void initEntityManager(EntityManager *entMan)
{

	entMan->entRef = 0;
	
	//entMan->entList = (Entity*)malloc(sizeof(Entity)*MAX_ENTITIES);

	int i;
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		initEntity(&entMan->entList[i]);
		entMan->entList[i].active = 0;
		entMan->entList[i].currFrame = 0;
	}

}

/*ALL*/

void updateAllEntities(EntityManager *entMan)
{
	int i;

	entMan->entList[0].update(&entMan->entList[0]); // player
	entMan->entList[1].update(&entMan->entList[1]); // skateboard

	for (i = 2; i < MAX_ENTITIES; i++)
	{
		if (entMan->entList[i].active && entMan->entList[i].onScreen)
		{
			entMan->entList[i].update(&entMan->entList[i]);
			scrollUp(&entMan->entList[i].position.y, &entMan->entList[i].sprite, PLAYER_SPEED, &entMan->entList[i], &entMan->entRef);
		}
	}
}

void drawAllEntities(EntityManager *entMan)
{
	int i;
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		if (entMan->entList[i].active && entMan->entList[i].onScreen) entMan->entList[i].draw(&entMan->entList[i]);
	}
}

/*SINGLE*/

Entity *initSingleEntity(EntityManager *entMan)
{
	int i;
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		if (!entMan->entList[i].active)
		{
			entMan->entList[i] = *initEntity(&entMan->entList[i]);
			entMan->entRef++;
			return (&entMan->entList[i]);
		}
	}
	return NULL;
}

void popEntList(EntityManager *entMan)
{
	int i;
	for (i = MAX_ENTITIES; i >= 1; i--)
	{
		if (entMan->entList[i].active)
		{
			entMan->entList[i].free(&entMan->entList[i], &entMan->entRef);
			break;
		}
	}
}