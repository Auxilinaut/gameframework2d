#include "entity_manager.h"

void initEntityManager(EntityManager *entMan)
{

	entMan->entRef = 0;

	int i;
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		initEntity(&entMan->entList[i]);
		entMan->entList[i].active = 0;
		entMan->entList[i].currFrame = 0;
	}

}

void updateAllEntities(EntityManager *entMan)
{
	int i;
	for (i = 0; i < entMan->entRef; i++)
	{
		entMan->entList[i].update(&entMan->entList[i]);
		scrollUp(&entMan->entList[i].position.y,2, &entMan->entList[i], &entMan->entRef);
	}
}

void drawAllEntities(EntityManager *entMan)
{
	int i;
	for (i = 0; i < entMan->entRef; i++)
	{
		entMan->entList[i].draw(&entMan->entList[i]);
	}
}

