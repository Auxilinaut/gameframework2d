#include <stdio.h>
#include <string.h>
#include "gf2d_text.h"
#include "entity_manager.h"

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

int countObstaclesInStr(char *obsBlock);
void loadObstacles(Level *lvl, EntityManager *entMan);
int countLevelsInFile(FILE *file);
void parseLevelFile(FILE *file, LevelList *lvlList);
LevelList *loadLevelFileToList(char *fileName);
Level *getLevelFromList(LevelList *lvlList, int id);
void loadLevelFile(LevelList *lvlList, char *file, EntityManager *entMan);