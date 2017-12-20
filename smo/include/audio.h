#include <SDL_mixer.h>
#include <stdio.h>

#define MAX_SFX 10

extern Mix_Music *music;
extern Mix_Chunk *sfx[MAX_SFX];

/**
* @brief runs Mix_OpenAudio() or errors
*/
void initAudio();

/**
* @brief loads extern music into memory
* @param fileName name of file loading music from
*/
void loadMusic(char *filename);

/**
* @brief loads extern sfx into memory
* @param index index of the sfx in array
* @param fileName string of file name
*/
void loadSfx(int index, char *filename);

/**
* @brief sets the needle
*/
void playMusic();

/**
* @brief makes a noise happen
* @param index the index of the sfx to play
*/
void playSfx(int index);
