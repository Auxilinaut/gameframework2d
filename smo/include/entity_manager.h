#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#define MAX_ENTITIES 500 + 1 //max + 1 for the player

#include "entity.h"
#include "physics.h"

typedef struct
{
	Entity entList[MAX_ENTITIES]; //todo: abstract player (currently using entList[0])
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

/**
* @brief initializes one entity by looking through entList for inactive
* @param entMan pointer to the entity manager
* @return pointer to initialized entity in entList
*/
Entity *initSingleEntity(EntityManager *entMan);

/**
* @brief frees last active entity in entList
* @param entMan pointer to the entity manager
*/
void popEntList(EntityManager *entMan);

#endif // !__ENTITY_MANAGER_H__