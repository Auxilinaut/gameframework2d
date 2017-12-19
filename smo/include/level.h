#include <stdio.h>
#include <string.h>
#include "gf2d_text.h"
#include "entity_manager.h"

#define MAX_COINS 10

typedef struct Level_S
{
	int id;
	Sprite *background;
	Entity *obstacles;
	int numObstacles;
	TextLine bgLine;
	TextLine bgmLine;
	TextBlock obsBlock;
	TextBlock obsBlockTemp;
}Level;

typedef struct LevelList_S
{
	Uint32	 refCount;
	TextLine fileName;
	Level	 *levels;
	int      numLevels;
}LevelList;

int countLevelsInFile(FILE *file);
int countObstaclesInStr(char *obsBlock);

void loadLevelFile(LevelList *lvlList, char *file, EntityManager *entMan);
void parseLevelFile(FILE *file, LevelList *lvlList);
void loadObstacles(Level *lvl, EntityManager *entMan);
LevelList getLevelListFromFile(char *fileName);
Level *getLevelFromList(LevelList *lvlList, int id);

void loadLevel(LevelList *lvlList, int id, Sprite *bg, EntityManager *entMan);

Entity *initCoin(Entity *coin, EntityManager *entMan, int x, int y);
void touchCoin(Entity *self, Entity *other);
void touchRamp(Entity *self, Entity *other);