#include "entity_manager.h";

typedef struct Player_S
{

	Entity *ent;
	int score;
	int highScore;


}Player;

void initPlayer(Player *p, EntityManager *entMan);

void movePlayer(Entity *ent);

void updatePlayer(Entity *ent);