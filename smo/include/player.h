#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity_manager.h"

#define PLAYER_SPEED 2.0
#define PLAYER_SPEED_HALF PLAYER_SPEED / 2
#define PLAYER_SPEED_MOD PLAYER_SPEED * 3

	extern int currScore;
	extern int highScore;

/**
* @brief initialize player entity
* @param ent pointer to player entity
* @param entMan pointer to entity manager
* @return pointer to player entity
*/
	Entity *initPlayer(Entity *ent, EntityManager *entMan);

/**
* @brief move player based on position
* @param ent pointer to player entity
*/
void movePlayer(Entity *ent);

/**
* @brief overload for player.update(), player-specific logic
* @param ent pointer to player entity
*/
void updatePlayer(Entity *ent);

/**
* @brief overload for player.update(), player-specific logic
* @param ent pointer to player entity
*/
void touchPlayer(Entity *self, Entity *other);

/**
* @brief give player some score, update high score
* @param score the amount of points to give to the player
*/
void updateScore(int score);

#endif