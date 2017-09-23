#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#define MAX_ENTITIES 1000 + 1 //max + 1 for the player

#include "entity.h"
#include "physics.h"

typedef struct
{
	Entity entList[MAX_ENTITIES]; //todo: abstract player
	int entRef;

}EntityManager;

/**
* @brief initializes the entity manager and its contained entities
* @param entMan pointer to the entity manager
* @return EntityManager only one of these needed
*/
void initEntityManager(EntityManager *entMan);

/**
* @brief updates every entity in the entity manager's entity list
* @param entMan pointer to the entity manager
*/
void updateAllEntities(EntityManager *entMan);

/**
* @brief draws every entity in the entity manager's entity list
* @param entMan pointer to the entity manager
*/
void drawAllEntities(EntityManager *entMan);

#endif // !__ENTITY_MANAGER_H__