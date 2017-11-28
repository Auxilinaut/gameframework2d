#include <SDL.h>
#include <stdio.h>

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "entity_manager.h"
#include "physics.h"
#include "player.h"
#include "skateboard.h"
#include "level.h"
#include "audio.h"


//#define SCREEN_HEIGHT_HALF 360
//#define SCREEN_HEIGHT_1POINT5 1080
//#define SCREEN_HEIGHT_2X 1440

/*----------------*/
/*---BEGIN MAIN---*/
/*----------------*/

int main(int argc, char *argv[])
{

	/*SPRITES*/
	Sprite *background; Vector2D backgroundPos[2];
	Sprite *mouse;
	Sprite *penguin;
	Sprite *sb;

	/*ENTITIES*/
	EntityManager entityManager;
	Player *player = (Player*)malloc(sizeof(Player));
	Skateboard *skateboard = (Skateboard*)malloc(sizeof(Skateboard));

	/*INPUT*/
	const Uint8 *keys;
	Vector4D mouseColor = { 255,100,255,200 };
	Bool clicking = 0;
	Bool typing = 0;
	int mx = 0, my = 0; //mouse pos
	float mf = 0; //mouse anim frame [0,16.0]


	/*LEVELS*/
	LevelList lvlList;

	/*OTHER*/
    int done = 0; //main while loop
	int i = 0; //generic iterator
	Uint32 time = SDL_GetTicks();
	Uint32 lastTime = NULL;
    
    /*program initialization*/

    init_logger("gf2d.log");

    slog("---==== BEGIN ====---");

    gf2d_graphics_initialize(
        "SMO",
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
        vector4d(0,0,0,255),
        0);

    gf2d_graphics_set_frame_delay(16);

    gf2d_sprite_init(300);

    SDL_ShowCursor(SDL_DISABLE);
    
    /*load stuff*/

    mouse = gf2d_sprite_load_all("smo/images/pointer.png",32, 32, 16);
	penguin = gf2d_sprite_load_all("smo/images/1797.png", 41, 42, 8);
	sb = gf2d_sprite_load_all("smo/images/skateboard.png", 40, 13, 5);

	/*initialize stuff*/

	initEntityManager(&entityManager);
	
	initPlayer(player, &entityManager);
	player->ent->animList = getAnimListFromFile("smo/anim/penguin.anim");
	player->ent->sprite = penguin;
	player->ent->frames = 64;
	setBounds(player->ent);

	initSkateboard(skateboard, &entityManager);
	skateboard->ent->animList = getAnimListFromFile("smo/anim/skateboard.anim");
	skateboard->ent->sprite = sb;
	skateboard->ent->frames = 15;

	initAudio();
	loadMusic("smo/audio/music.ogg");
	//playMusic();

	lvlList = getLevelListFromFile("smo/level/level.lvl");
	loadLevelFile(&lvlList, "smo/level/level.lvl", &entityManager); //loads backgrounds, bgm, and obstacle data
	slog("numLevels %d", lvlList.numLevels);

	background = lvlList.levels[0].background;
	backgroundPos[0] = vector2d(0, 0);
	backgroundPos[1] = vector2d(0, SCREEN_HEIGHT);

	//uncomment to slog levels/obstacles
	/*int j = 0;
	while (j < lvlList.numLevels)
	{
		int k = 0;
		slog("level %d numObstacles %d", j, lvlList.levels[j].numObstacles);
		slog("level %d bg %s", j, lvlList.levels[j].background->filepath);
		while (k < lvlList.levels[j].numObstacles)
		{
			slog("level %d obstacle %s", j, lvlList.levels[j].obstacles[k].name);
			k++;
		}
		j++;
	}*/

    /*main game loop*/

	while (!done)
	{

		/*UPDATE*/

		SDL_PumpEvents(); //update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); //get the keyboard state for this frame
		time = SDL_GetTicks();

		//update mouse anim frame
		mf += 0.1f;
		if (mf >= 16.0)
		{
			mf = 0;
		}

		SDL_GetMouseState(&mx, &my);

		if (keys[SDL_SCANCODE_1])
		{
			if (!typing)
			{
				loadLevel(&lvlList, 0, background, &entityManager);
				typing = 1;
			}
		}
		else if (keys[SDL_SCANCODE_2])
		{
			if (!typing)
			{
				loadLevel(&lvlList, 1, background, &entityManager);
				typing = 1;
			}
		}
		else if (keys[SDL_SCANCODE_3])
		{
			if (!typing)
			{
				loadLevel(&lvlList, 2, background, &entityManager);
				typing = 1;
			}
		}
		else if (keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP])
		{	
			if (!typing)
			{
				jump(player->ent);
				typing = 1;
			}
		}
		else if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
		{
			if (!typing)
			{
				turn(&player->ent->direction, 0);
				typing = 1;
			}
		}
		else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
		{
			if (!typing)
			{
				turn(&player->ent->direction, 1);
				typing = 1;
			}
		}
		else
		{
			typing = 0;
		}

		updateAllEntities(&entityManager);

		/*DRAW*/
        
        gf2d_graphics_clear_screen(); //clears drawing buffers

        //all drawing should happen between clear_screen and next_frame

        //backgrounds drawn first
		for (i = 0; i < 2; i++)
		{
			gf2d_sprite_draw_image(background, backgroundPos[i]);
			scrollUp(&backgroundPos[i].y, background, PLAYER_SPEED, NULL, NULL);
		}

		//entities next
		drawAllEntities(&entityManager);
            
        //UI elements last
        gf2d_sprite_draw(
            mouse,
            vector2d(mx,my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);

        gf2d_graphics_next_frame(); //render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; //exit condition

        printf("Rendering at %f FPS\n",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

/*---END MAIN---*/
