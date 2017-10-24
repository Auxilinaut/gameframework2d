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
		lvl->obstacles[i] = *initEntity(&lvl->obstacles[i]);
		lvl->obstacles[i].onScreen = 0;

		gf2d_line_cpy(lvl->obstacles[i].name, obstacles);
		//slog("name of obstacle #%d: %s", i, obs->name);

		//slog("attempting to load sprite for %s", obs->name);
		if (strcmp(obstacles, "cone") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/cone.png");
		}
		else if (strcmp(obstacles, "hydrant") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/hydrant.png");
		}
		else if (strcmp(obstacles, "sign") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/sign.png");
		}
		else if (strcmp(obstacles, "fish") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_all("smo/images/fish.png", 32, 32, 3);
			lvl->obstacles[i].animList = getAnimListFromFile("smo/anim/fish.anim");
			if (strcmp(lvl->obstacles[i].currAnim, "up") != 0) setEntityAnim(&lvl->obstacles[i], "up");
			lvl->obstacles[i].frames = 3;
		}
		else if (strcmp(obstacles, "snake") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_all("smo/images/snake.png", 32, 32, 3);
			lvl->obstacles[i].animList = getAnimListFromFile("smo/anim/snake.anim");
			if (strcmp(lvl->obstacles[i].currAnim, "up") != 0) setEntityAnim(&lvl->obstacles[i], "up");
			lvl->obstacles[i].frames = 3;
		}
		else if (strcmp(obstacles, "frog") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_all("smo/images/frog.png", 32, 32, 3);
			lvl->obstacles[i].animList = getAnimListFromFile("smo/anim/frog.anim");
			if (strcmp(lvl->obstacles[i].currAnim, "up") != 0) setEntityAnim(&lvl->obstacles[i], "up");
			lvl->obstacles[i].frames = 3;
		}
		else if (strcmp(obstacles, "asteroidone") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/asteroidone.png");
		}
		else if (strcmp(obstacles, "asteroidtwo") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/asteroidtwo.png");
		}
		else if (strcmp(obstacles, "planetone") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/planetone.png");
		}
		else if (strcmp(obstacles, "planettwo") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/planettwo.png");
		}
		else if (strcmp(obstacles, "star") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/star.png");
		}
		else if (strcmp(obstacles, "blackhole") == 0)
		{
			lvl->obstacles[i].sprite = gf2d_sprite_load_image("smo/images/blackhole.png");
		}
		setBounds(&lvl->obstacles[i]);
		slog("test sprite %s frame_h %d", lvl->obstacles[i].name, lvl->obstacles[i].sprite->frame_h);
		obstacles = strtok(NULL, ",");
		i++;
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
			fscanf(file, "%s", &levels->bgLine);
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

LevelList getLevelListFromFile(char *fileName)
{
	FILE *file;
	LevelList levelList;
	int count;
	file = fopen(fileName, "r");
	if (!file)
	{
		slog("failed to open level file: %s", fileName);
		return;
	}

	gf2d_line_cpy(levelList.fileName, fileName);

	count = countLevelsInFile(file);
	levelList.levels = (Level*)malloc(sizeof(Level)*count);
	memset(levelList.levels, 0, sizeof(Level)*count);
	levelList.numLevels = count;

	parseLevelFile(file, &levelList);

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
	int i, obsCount = 0;
	//char *obstacles;

	if (!lvlList)
	{
		slog("no level list specified for level file loading");
		return;
	}

	*lvlList = getLevelListFromFile(file);

	if (!lvlList) return; //should have logged the error already

	for (i = 0; i < lvlList->numLevels; i++)
	{
		lvlList->levels[i].id = i;

		//handle bg loading
	    lvlList->levels[i].background = gf2d_sprite_load_image(lvlList->levels[i].bgLine);

		//handle bgm loading

		//handle obstacle loading
		gf2d_block_cpy(lvlList->levels[i].obsBlockTemp, lvlList->levels[i].obsBlock);
		obsCount = countObstaclesInStr(lvlList->levels[i].obsBlockTemp);
		lvlList->levels[i].numObstacles = obsCount;

		loadObstacles(&lvlList->levels[i], entMan);
	}
}

void loadLevel(LevelList *lvlList, int id, Sprite *bg, EntityManager *entMan)
{
	slog("loading lvl %d", id);
	slog("lvl %d bg %s", id, lvlList->levels[id].background->filepath);

	*bg = *lvlList->levels[id].background;
	for (int i = 2; i < lvlList->levels[id].numObstacles + 2; i++)
	{
		entMan->entList[i] = *initEntity(&lvlList->levels[id].obstacles[i-2]);
		entMan->entList[i].position.x = rand() % SCREEN_WIDTH;
		entMan->entList[i].position.y = rand() % SCREEN_HEIGHT;
	}
}