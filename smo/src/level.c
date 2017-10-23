#include "level.h"
#include "simple_logger.h"

//like fgets but for strings
//altered functionality originally made by Mark Wilkins on StackOverflow
char *sgets(char * str, int num, char **input)
{
	char *next = *input;
	int  numread = 0;
	int isComma;

	while (numread + 1 < num && *next) {
		isComma = (*next == ',');
		*str++ = *next++;
		numread++;
		// comma terminates the line and is not included
		if (isComma)
		{
			*str--;
			break;
		}
	}

	if (numread == 0)
		return NULL;  // "eof"

					  // must have hit the null terminator or end of line
	*str = '\0';  // null terminate this string
				  // set up input for next call
	*input = next;
	return str;
}

int countObstaclesInStr(char *obsBlock)
{
	char buf[512];
	int count = 0;
	if (!obsBlock) return 0; //just in case
	while (sgets(buf, sizeof(buf), &obsBlock))
	{
		count++;
	}
	return count;
}

void loadObstacles(Level *lvl, EntityManager *entMan)
{
	int i = 0;
	char *obstacles = lvl->obsBlock;
	char buf[128];
	gf2d_line_cpy(buf, obstacles);

	lvl->obstacles = (Entity*)malloc(sizeof(Entity) * lvl->numObstacles);
	memset(lvl->obstacles, 0, sizeof(Entity) * lvl->numObstacles);

	obstacles = strtok(buf, ",");
	while (obstacles != NULL)
	{
		lvl->obstacles = initSingleEntity(entMan);
		lvl->obstacles->active = 0;

		gf2d_line_cpy(lvl->obstacles->name, obstacles);
		slog("name of obstacle #%d: %s", i, lvl->obstacles->name);

		lvl->obstacles++;
		obstacles = strtok(NULL, ",");
		i++;
	}

	for (i = 0; i < lvl->numObstacles; i++)
	{

	}
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

void parseLevelFile(FILE *file, LevelList *lvlList)
{
	Level *levels;
	char buf[512];
	if (!file)return;
	rewind(file);
	levels = lvlList->levels;
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
			fscanf(file, "%s", &levels->bgmLine);
			continue;
		}
		if (strcmp(buf, "obstacles:") == 0)
		{
			fscanf(file, "%s", &levels->obsBlock);
			continue;
		}
		if (strcmp(buf, "level:") == 0)
		{
			levels++;
			continue;
		}
		if (levels < lvlList->levels)
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

Level *getLevelFromList(LevelList *lvlList, int id)
{
	int i;
	if (!lvlList)
	{
		slog("no level list provided");
		return NULL;
	}
	if (!id)
	{
		slog("no level id provided");
		return NULL;
	}
	for (i = 0; i < lvlList->numLevels; i++)
	{
		if (lvlList->levels[i].id == id)
		{
			return &lvlList->levels[i];
		}
	}
	return NULL;
}

void loadLevelFile(LevelList *lvlList, char *file, EntityManager *entMan)
{
	int i, j, obsCount = 0;
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
		//handle bg loading
		lvlList->levels[i].background = gf2d_sprite_load_image(lvlList->levels[i].bgLine);
		
		//handle bgm loading

		//handle obstacle loading
		gf2d_block_cpy(lvlList->levels[i].obsBlockTemp, lvlList->levels[i].obsBlock);
		obsCount = countObstaclesInStr(lvlList->levels[i].obsBlockTemp);
		slog("Level #%d obstacle count: %d", (i + 1), obsCount);

		loadObstacles(&lvlList->levels[i], entMan);

		for (j = 0; j < obsCount; j++)
		{
			
		}
	}
}