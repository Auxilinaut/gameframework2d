#include "audio.h"
#include "simple_logger.h"

Mix_Music *music;
Mix_Chunk *sfx[MAX_SFX];

void initAudio()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		slog("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
}

void loadMusic(char *filename)
{
	music = Mix_LoadMUS(filename);
	if (music == NULL)
	{
		slog("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
}

void loadSfx(int index, char *filename)
{
	sfx[index] = Mix_LoadWAV(filename);
	if (sfx[index] == NULL)
	{
		slog("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
}

void playMusic()
{
	Mix_PlayMusic(music, -1);
}

void playSfx(int index)
{
	Mix_PlayChannel(-1, sfx[index], 0);
}
