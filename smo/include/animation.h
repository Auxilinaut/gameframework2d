/**
* Animation handler for Entities
* Derived from Actor properties courtesy of EngineerOfLies
*/

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <stdio.h>
#include "gf2d_sprite.h"
#include "entity.h"

#define MAX_ANIMATION_LISTS MAX_ENTITIES

typedef enum AnimType_E
{
	AT_NONE,
	AT_LOOP,
	AT_PASS
}AnimType;

typedef enum AnimReturnType_E
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
typedef struct AnimList_S
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

/**
* @brief Opens the specified .anim file to parse into an AnimList
* @param fileName The name of the file
* @returns The AnimList after loading and parsing
*/
AnimList *getAnimListFromFile(char *fileName);

/**
* @brief Looks in AnimList for a specific animation name
* @param al The AnimList to look in
* @param name The name of the animation desired
* @returns The Anim found or NULL on error
*/
Anim *getAnimFromList(AnimList *al, char *name);

/**
* @brief Gets the first frame of an animation
* @param al The AnimList to look in
* @param name The name of the animation desired
* @returns The starting frame for the animation
*/
float getAnimFrame(AnimList *al, char *name);

/**
* @brief checks next frame for current status of an Anim
* @param al The AnimList to look in
* @param frame the frame to look at
* @param frame the anim name to check
* @returns AnimReturnType
*/
AnimReturnType findNextFrame(AnimList *al, float * frame, char *name);

/**
* @brief loads an anim file into memory
* @param ent which entity's anims are changing
* @param file string containing the file name of the anim
*/
void loadEntityAnimFile(struct Entity_S *ent, char *file);

/**
* @brief sets the animation of one entity
* @param ent which entity's anims are changing
* @param anim string containing the name of the anim
*/
void setEntityAnim(struct Entity_S *ent, char *anim);

/**
* @brief continues an entity's animations
* @param ent which entity's anims are changing
*/
void nextEntFrame(struct Entity_S *ent);

#endif