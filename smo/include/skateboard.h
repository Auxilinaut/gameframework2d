#ifndef __SKATEBOARD_H__
#define __SKATEBOARD_H__

#include "player.h"

typedef struct Skateboard_S
{
	Entity *ent;
}Skateboard;

/**
* @brief initialize skateboard struct
* @param sb skateboard to player
* @param entMan the entity manager
*/
void initSkateboard(Skateboard *sb, EntityManager *entMan);

/**
* @brief move skateboard based on position
* @param ent pointer to skateboard entity
*/
void moveSkateboard(Entity *ent);

/**
* @brief overload for skateboard.update(), skateboard-specific logic
* @param ent pointer to skateboard entity
*/
void updateSkateboard(Entity *ent);

#endif