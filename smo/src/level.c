#include "level.h"
#include "simple_logger.h"

void loadObstacles(Level lvl, EntityManager *entMan)
{
	//*lvl.obstacles = 
}

int countLevelsInFile(FILE *file)
{
	char buf[512];
	int count = 0;
	if (!file)return 0;
	rewind(file);
	while (fscanf(file, "%s", buf) != EOF)
	{
		if (strcmp(buf, "level:") == 0)
		{
			count++;
		}
		fgets(buf, sizeof(buf), file);
	}
	return count;
}

void parseLevelFile(FILE *file, LevelList *levelList)
{
	Level *levels;
	char buf[512];
	if (!file)return;
	rewind(file);
	levels = levelList->levels;
	levels--;
	while (fscanf(file, "%s", buf) != EOF)
	{
		if (strcmp(buf, "bg:") == 0)
		{
			fscanf(file, "%s", (char*)&levels->bgLine);
			continue;
		}
		if (strcmp(buf, "bgm:") == 0)
		{
			fscanf(file, "%i", &levels->bgmLine);
			continue;
		}
		if (strcmp(buf, "obstacles:") == 0)
		{
			fscanf(file, "%i", &levels->obsBlock);
			continue;
		}
		if (strcmp(buf, "level:") == 0)
		{
			levels++;
			continue;
		}
		if (levels < levelList->levels)
		{
			slog("file formatting error, expect level: tag before rest of data");
			continue;
		}
		fgets(buf, sizeof(buf), file);
	}
}

LevelList *loadLevelFileToList(char *fileName)
{
	FILE *file;
	LevelList *levelList = (LevelList*)malloc(sizeof(LevelList));
	int count;
	file = fopen(fileName, "r");
	if (!file)
	{
		slog("failed to open level file: %s", fileName);
		return NULL;
	}

	gf2d_line_cpy(levelList->fileName, fileName);

	count = countLevelsInFile(file);
	levelList->levels = (Level*)malloc(sizeof(Level)*count);
	memset(levelList->levels, 0, sizeof(Level)*count);
	levelList->numLevels = count;

	parseLevelFile(file, levelList);

	fclose(file);
	return levelList;
}

Level *getLevelFromList(LevelList *ll, int id)
{
	int i;
	if (!ll)
	{
		slog("no level list provided");
		return NULL;
	}
	if (!id)
	{
		slog("no level id provided");
		return NULL;
	}
	for (i = 0; i < ll->numLevels; i++)
	{
		if (ll->levels[i].id == id)
		{
			return &ll->levels[i];
		}
	}
	return NULL;
}

void loadLevelFile(LevelList *lvlList, char *file)
{
	int i;
	if (!lvlList)
	{
		slog("no level list specified for level file loading");
		return;
	}
	lvlList = loadLevelFileToList(file);
	if (!lvlList)
	{
		return;// should have logged the error already
	}
	for (i = 0; i < lvlList->numLevels; i++)
	{
		lvlList->levels[i].background = gf2d_sprite_load_image(lvlList->levels[i].bgLine);
		//handle bgm loading
		//handle obstacle loading
	}
}