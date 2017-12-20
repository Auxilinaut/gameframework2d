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

/**
* @brief parses file for level count
* @param file pointer to file
* @return int level count
*/
int countLevelsInFile(FILE *file);

/**
* @brief parses string for obstacle count
* @param obsBlock pointer to TextBlock to be checked
* @return int obstacle count
*/
int countObstaclesInStr(char *obsBlock);

/**
* @brief parses string for obstacle count
* @param lvlList pointer to LevelList
* @param file file name
* @param entMan pointer to EntityManager
*/
void loadLevelFile(LevelList *lvlList, char *file, EntityManager *entMan);

/**
* @brief parses level file into LevelList
* @param file pointer to file
* @param lvlList pointer to LevelList
*/
void parseLevelFile(FILE *file, LevelList *lvlList);

/**
* @brief loads a level's obstacles into memory
* @param lvl pointer to Level
* @param entMan pointer to EntityManager
*/
void loadObstacles(Level *lvl, EntityManager *entMan);

/**
* @brief counts, loads, and parses level file into a LevelList
* @param fileName string for file name
* @return LevelList
*/
LevelList getLevelListFromFile(char *fileName);

/**
* @brief retrieves a Level from a LevelList
* @param lvlList pointer to LevelList
* @param id index of Level in the list
* @return Level pointer to Level
*/
Level *getLevelFromList(LevelList *lvlList, int id);

/***
* @brief changes current level to the specified level
* @param lvlList pointer to LevelList
* @param id index of Level in the list
* @param bg pointer to background Sprite
* @param entMan pointer to EntityManager
*/
void loadLevel(LevelList *lvlList, int id, Sprite *bg, EntityManager *entMan);

/**
* @brief makes a new coin at the specified location
* @param coin pointer to coin Entity
* @param entMan pointer to EntityManager
* @param x x position
* @param y y position
* @return Level pointer to Level
*/
Entity *initCoin(Entity *coin, EntityManager *entMan, int x, int y);

/***
* @brief collision functionality for coin
* @param self pointer to the coin Entity
* @param other pointer to a Player
*/
void touchCoin(Entity *self, Entity *other);