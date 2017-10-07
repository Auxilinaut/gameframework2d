/**
* Animation handler for Entities
* Anim (formerly Actor) properties courtesy of EngineerOfLies
*/

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <stdio.h>
#include "gf2d_sprite.h"
#include "entity_manager.h"

#define MAX_ANIMATION_LISTS MAX_ENTITIES

typedef enum
{
	AT_NONE,
	AT_LOOP,
	AT_PASS
}AnimType;

typedef enum
{
	ART_ERROR,
	ART_NORMAL,
	ART_END,
	ART_START
}AnimReturnType;

/**
* @brief An individual animation
*/
typedef struct Anim_S
{
	TextLine name;
	int startFrame;
	int endFrame;
	float frameRate;
	AnimType type;
}Anim;

/**
* @brief List of animations tied to each Entity
*/
typedef struct
{
	Uint32	 refCount;
	TextLine fileName;
	Anim	 *animations;
	int      numAnimations;
	TextLine sprite;
	int      frameWidth;
	int      frameHeight;
	int      framesPerLine;
	Vector2D scale;
	Vector4D color;
}AnimList;

int countAnimsInFile(FILE *file);

/**
* @brief Parses the contents of animation file into an AnimList
* @param file The file to be parsed
* @param animList The animation list to parse into
*/
void parseAnimFile(FILE *file, AnimList *animList);

AnimList *loadAnimFileToList(char *fileName);

Anim *getAnimFromList(AnimList *al, char *name);

float setAnim(AnimList *al, char *name);

AnimReturnType findNextFrame(AnimList *al, float * frame, char *name);

void loadEntityAnimFile(Entity *ent, char *file);

void setEntityAnim(Entity *ent, char *anim);

void nextEntFrame(Entity *ent);

#endif