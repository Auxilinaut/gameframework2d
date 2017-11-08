#include <SDL_mixer.h>
#include <stdio.h>

#define MAX_SFX 10

extern Mix_Music *music;
extern Mix_Chunk *sfx[MAX_SFX];

void initAudio();

void loadMusic(char *filename);

void loadSfx(int index, char *filename);

void playMusic();

void playSfx(int index);
