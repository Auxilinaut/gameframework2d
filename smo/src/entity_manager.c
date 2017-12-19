#include "entity_manager.h"
#include "player.h"
#include "simple_logger.h"

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

	if (entMan->entList[0].alive)
	{
		// player
		setBounds(&entMan->entList[0]);
		entMan->entList[0].update(&entMan->entList[0]);

		// skateboard
		entMan->entList[1].update(&entMan->entList[1]);
	}

	// everything else
	for (i = 2; i < MAX_ENTITIES; i++)
	{
		if (entMan->entList[i].active)
		{
			if (entMan->entList[i].alive)
			{
				setBounds(&entMan->entList[i]);
				if (checkCollision(entMan->entList[0].bounds, entMan->entList[i].bounds))
				{
					entMan->entList[0].collider = &entMan->entList[i];
					entMan->entList[0].colliding = true;
					entMan->entList[i].colliding = true;
					entMan->entList[i].touch(&entMan->entList[i], &entMan->entList[0]);
					slog("collision detected between player and entity %d", i);
				}
				scrollUp(&entMan->entList[i].position.y, entMan->entList[i].sprite, PLAYER_SPEED, &entMan->entList[i], &entMan->entRef);
			}

			entMan->entList[i].update(&entMan->entList[i]);
		}
	}
}

void drawAllEntities(EntityManager *entMan)
{
	int i;
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		if (entMan->entList[i].active && entMan->entList[i].alive) entMan->entList[i].draw(&entMan->entList[i]);
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
			entMan->entList[i] = *initEntity(&(entMan->entList[i]));
			entMan->entRef++;
			return (&(entMan->entList[i]));
		}
	}
	return NULL;
}

void popEntList(EntityManager *entMan)
{
	int i;
	for (i = MAX_ENTITIES; i >= 2; i--)
	{
		if (entMan->entList[i].active)
		{
			entMan->entList[i].free(&entMan->entList[i], &entMan->entRef);
			break;
		}
	}
}