#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity_manager.h"

#define PLAYER_SPEED 2.0
#define PLAYER_SPEED_HALF PLAYER_SPEED / 2

typedef struct Player_S
{
	Entity *ent;
	int score;
	int highScore;
}Player;

/**
* @brief initialize player struct
* @param plr pointer to player
*/
void initPlayer(Player *plr, EntityManager *entMan);

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
* @brief give player some score, update high score
* @param plr pointer to the player struct
* @param score the amount of points to give to the player
*/
void updateScore(Player *plr, int score);

#endif