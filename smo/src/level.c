#include "level.h"

void loadBackground(Level lvl, TextLine bgFileName)
{
	lvl.background = gf2d_sprite_load_image(bgFileName);
}

void loadObstacles(Level lvl, EntityManager *entMan)
{
	//*lvl.obstacles = 
}

#include "simple_logger.h"

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
			fscanf(file, "%s", (char*)&levelList->bg);
			continue;
		}
		if (strcmp(buf, "frameWidth:") == 0)
		{
			fscanf(file, "%i", &levelList->bgm);
			continue;
		}
		if (strcmp(buf, "frameHeight:") == 0)
		{
			fscanf(file, "%i", &levelList->frameHeight);
			continue;
		}
		if (strcmp(buf, "framesPerLine:") == 0)
		{
			fscanf(file, "%i", &levelList->framesPerLine);
			continue;
		}
		if (strcmp(buf, "scale:") == 0)
		{
			fscanf(file, "%lf,%lf", &levelList->scale.x, &levelList->scale.y);
			continue;
		}
		if (strcmp(buf, "color:") == 0)
		{
			fscanf(file, "%lf,%lf,%lf,%lf", &levelList->color.x, &levelList->color.y, &levelList->color.z, &levelList->color.w);
			continue;
		}
		/*if (strcmp(buf, "colorSpecial:") == 0)
		{
		fscanf(file, "%lf,%lf,%lf,%lf", &levelList->colorSpecial.x, &levelList->colorSpecial.y, &levelList->colorSpecial.z, &levelList->colorSpecial.w);
		continue;
		}*/
		if (strcmp(buf, "level:") == 0)
		{
			levels++;
			fscanf(file, "%s", (char*)&levels->name);
			continue;
		}
		if (levels < levelList->levels)
		{
			slog("file formatting error, expect level: tag before rest of data");
			continue;
		}
		if (strcmp(buf, "type:") == 0)
		{
			fscanf(file, "%s", buf);
			if (strcmp(buf, "loop") == 0)
			{
				levels->type = AT_LOOP;
				continue;
			}
			if (strcmp(buf, "pass") == 0)
			{
				levels->type = AT_PASS;
				continue;
			}
			continue;
		}
		if (strcmp(buf, "startFrame:") == 0)
		{
			fscanf(file, "%i", &levels->startFrame);
			continue;
		}
		if (strcmp(buf, "endFrame:") == 0)
		{
			fscanf(file, "%i", &levels->endFrame);
			continue;
		}
		if (strcmp(buf, "frameRate:") == 0)
		{
			fscanf(file, "%f", &levels->frameRate);
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
		slog("failed to open levelation file: %s", fileName);
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

Level *getLevelFromList(LevelList *al, char *name)
{
	int i;
	if (!al)
	{
		slog("no levelation list provided");
		return NULL;
	}
	if (!name)
	{
		slog("no levelation name provided");
		return NULL;
	}
	for (i = 0; i < al->numLevels; i++)
	{
		if (gf2d_line_cmp(al->levels[i].name, name) == 0)
		{
			return &al->levels[i];
		}
	}
	return NULL;
}

float getLevelFrame(LevelList *al, char *name)
{
	Level *level;
	level = getLevelFromList(al, name);
	if (!level)
	{
		slog("no level found by name %s", name);
		return ART_ERROR;
	}
	return level->startFrame;
}

LevelReturnType findNextFrame(LevelList *al, float * frame, char *name)
{
	Level *level;
	if (!frame)
	{
		slog("missing frame data");
		return ART_ERROR;
	}
	level = getLevelFromList(al, name);
	if (!level)
	{
		slog("no level found by name %s", name);
		return ART_ERROR;
	}
	*frame += level->frameRate;
	if (*frame >= level->endFrame)
	{
		switch (level->type)
		{
		case AT_NONE:
			break;
		case AT_LOOP:
			*frame = level->startFrame;
			break;
		case AT_PASS:
			*frame = level->endFrame;
			return ART_END;
		}
	}
	return ART_NORMAL;
}

void loadEntityLevelFile(struct Entity_S *ent, char *file)
{
	if (!ent)
	{
		slog("no entity specified for levelation file loading");
		return;
	}
	ent->levelList = loadLevelFileToList(file);
	if (!ent->levelList)
	{
		return;// should have logged the error already
	}
	vector4d_copy(ent->colorShift, ent->levelList->color);
	ent->sprite = gf2d_sprite_load_all(
		ent->levelList->sprite,
		ent->levelList->frameWidth,
		ent->levelList->frameHeight,
		ent->levelList->framesPerLine);
}

void setEntityLevel(struct Entity_S *ent, char *level)
{
	if (!ent)return;
	ent->currFrame = getLevelFrame(ent->levelList, level);
	gf2d_line_cpy(ent->currLevel, level);
}

void nextEntFrame(struct Entity_S *ent)
{
	if (!ent)return;
	if (ent->levelList->numLevels) (LevelReturnType)(ent->levelRetType) = findNextFrame(ent->levelList, &ent->currFrame, ent->currLevel);
}