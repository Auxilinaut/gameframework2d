#include <stdio.h>
#include "gf2d_text.h"
#include "entity_manager.h"

typedef struct Level_S
{
	Sprite *background;
	Entity *obstacles;
}Level;

typedef struct LevelList_S
{
	Uint32	 refCount;
	TextLine fileName;
	Level	 *levels;
	int      numLevels;
}LevelList;