#include "animation.h"
#include "gf2d_text.h"
#include "simple_logger.h"

int countAnimsInFile(FILE *file)
{
	char buf[512];
	int count = 0;
	if (!file)return 0;
	rewind(file);
	while (fscanf(file, "%s", buf) != EOF)
	{
		if (strcmp(buf, "anim:") == 0)
		{
			count++;
		}
		fgets(buf, sizeof(buf), file);
	}
	return count;
}

void parseAnimFile(FILE *file, AnimList *animList)
{
	Anim *animations;
	char buf[512];
	if (!file)return;
	rewind(file);
	animations = animList->animations;
	animations--;
	while (fscanf(file, "%s", buf) != EOF)
	{
		if (strcmp(buf, "sprite:") == 0)
		{
			fscanf(file, "%s", (char*)&animList->sprite);
			continue;
		}
		if (strcmp(buf, "frameWidth:") == 0)
		{
			fscanf(file, "%i", &animList->frameWidth);
			continue;
		}
		if (strcmp(buf, "frameHeight:") == 0)
		{
			fscanf(file, "%i", &animList->frameHeight);
			continue;
		}
		if (strcmp(buf, "framesPerLine:") == 0)
		{
			fscanf(file, "%i", &animList->framesPerLine);
			continue;
		}
		if (strcmp(buf, "scale:") == 0)
		{
			fscanf(file, "%lf,%lf", &animList->scale.x, &animList->scale.y);
			continue;
		}
		if (strcmp(buf, "color:") == 0)
		{
			fscanf(file, "%lf,%lf,%lf,%lf", &animList->color.x, &animList->color.y, &animList->color.z, &animList->color.w);
			continue;
		}
		/*if (strcmp(buf, "colorSpecial:") == 0)
		{
			fscanf(file, "%lf,%lf,%lf,%lf", &animList->colorSpecial.x, &animList->colorSpecial.y, &animList->colorSpecial.z, &animList->colorSpecial.w);
			continue;
		}*/
		if (strcmp(buf, "anim:") == 0)
		{
			animations++;
			fscanf(file, "%s", (char*)&animations->name);
			continue;
		}
		if (animations < animList->animations)
		{
			slog("file formatting error, expect anim: tag before rest of data");
			continue;
		}
		if (strcmp(buf, "type:") == 0)
		{
			fscanf(file, "%s", buf);
			if (strcmp(buf, "loop") == 0)
			{
				animations->type = AT_LOOP;
				continue;
			}
			if (strcmp(buf, "pass") == 0)
			{
				animations->type = AT_PASS;
				continue;
			}
			continue;
		}
		if (strcmp(buf, "startFrame:") == 0)
		{
			fscanf(file, "%i", &animations->startFrame);
			continue;
		}
		if (strcmp(buf, "endFrame:") == 0)
		{
			fscanf(file, "%i", &animations->endFrame);
			continue;
		}
		if (strcmp(buf, "frameRate:") == 0)
		{
			fscanf(file, "%f", &animations->frameRate);
			continue;
		}
		fgets(buf, sizeof(buf), file);
	}
}

AnimList *loadAnimFileToList(char *fileName)
{
	FILE *file;
	AnimList *animList = (AnimList*)malloc(sizeof(AnimList));
	int count;
	file = fopen(fileName, "r");
	if (!file)
	{
		slog("failed to open animation file: %s", fileName);
		return NULL;
	}

	gf2d_line_cpy(animList->fileName, fileName);

	count = countAnimsInFile(file);
	animList->animations = (Anim*)malloc(sizeof(Anim)*count);
	memset(animList->animations, 0, sizeof(Anim)*count);
	animList->numAnimations = count;

	parseAnimFile(file, animList);

	fclose(file);
	return animList;
}

Anim *getAnimFromList(AnimList *al, char *name)
{
	int i;
	if (!al)
	{
		slog("no animation list provided");
		return NULL;
	}
	if (!name)
	{
		slog("no animation name provided");
		return NULL;
	}
	for (i = 0; i < al->numAnimations; i++)
	{
		if (gf2d_line_cmp(al->animations[i].name, name) == 0)
		{
			return &al->animations[i];
		}
	}
	return NULL;
}

float getAnimFrame(AnimList *al, char *name)
{
	Anim *anim;
	anim = getAnimFromList(al, name);
	if (!anim)
	{
		slog("no anim found by name %s", name);
		return ART_ERROR;
	}
	return anim->startFrame;
}

AnimReturnType findNextFrame(AnimList *al, float * frame, char *name)
{
	Anim *anim;
	if (!frame)
	{
		slog("missing frame data");
		return ART_ERROR;
	}
	anim = getAnimFromList(al, name);
	if (!anim)
	{
		slog("no anim found by name %s", name);
		return ART_ERROR;
	}
	*frame += anim->frameRate;
	if (*frame >= anim->endFrame)
	{
		switch (anim->type)
		{
		case AT_NONE:
			break;
		case AT_LOOP:
			*frame = anim->startFrame;
			break;
		case AT_PASS:
			*frame = anim->endFrame;
			return ART_END;
		}
	}
	return ART_NORMAL;
}

void loadEntityAnimFile(struct Entity_S *ent, char *file)
{
	if (!ent)
	{
		slog("no entity specified for animation file loading");
		return;
	}
	ent->animList = loadAnimFileToList(file);
	if (!ent->animList)
	{
		return;// should have logged the error already
	}
	vector4d_copy(ent->colorShift, ent->animList->color);
	ent->sprite = gf2d_sprite_load_all(
		ent->animList->sprite,
		ent->animList->frameWidth,
		ent->animList->frameHeight,
		ent->animList->framesPerLine);
}

void setEntityAnim(struct Entity_S *ent, char *anim)
{
	if (!ent)return;
	ent->currFrame = getAnimFrame(ent->animList, anim);
	gf2d_line_cpy(ent->currAnim, anim);
}

void nextEntFrame(struct Entity_S *ent)
{
	if (!ent)return;
	if (ent->animList->numAnimations) (AnimReturnType)(ent->animRetType) = findNextFrame(ent->animList, &ent->currFrame, ent->currAnim);
}