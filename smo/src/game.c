#include <Windows.h>
#include <stdio.h>
#include <SDL.h>

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "entity_manager.h"
#include "physics.h"
#include "player.h"
#include "skateboard.h"
#include "level.h"
#include "audio.h"

/*----------------*/
/*---BEGIN MAIN---*/
/*----------------*/

#ifdef _DEBUG
int main(int argc, char *argv[])
#endif
#ifdef NDEBUG
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
{
	/*SPRITES*/
	Sprite *background; Vector2D backgroundPos[2];
	Sprite *mouse;
	Sprite *penguin;
	Sprite *sb;
	Sprite *coinSpr;

	/*ENTITIES*/
	EntityManager entityManager;
	Entity *player = malloc(sizeof(Entity));
	Skateboard *skateboard = (Skateboard*)malloc(sizeof(Skateboard));
	Entity coins[MAX_COINS];

	/*INPUT*/
	const Uint8 *keys;
	Vector4D mouseColor = { 255,100,255,200 };
	Bool clicking = 0;
	Bool typing = 0;
	int mx = 0, my = 0; //mouse pos
	float mf = 0; //mouse anim frame [0,16.0]

	/*LEVELS*/
	LevelList lvlList; //each level defines entities, background, and bgm 
	int currLevel = 0;

	/*UI*/
	int state = 0; 
	TextLine scoreStr;
	SDL_Surface *textSurface = NULL;
	SDL_Point p; //point check for mouse
	SDL_Rect rStart = (SDL_Rect) { .x = 0, .y = 0, .w = 100, .h = 30 }; //start button bounds
	SDL_Rect rExit = (SDL_Rect) { .x = 0, .y = 30, .w = 100, .h = 30 }; //exit button bounds

	/*OTHER*/
    int done = 0; //main while loop
	int i = 0; //generic iterator
    
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

	if (TTF_Init() < 0)
	{
		slog("error initializing ttf");
	}

	TTF_Font* font = TTF_OpenFont("smo/fonts/OpenSans-Bold.ttf", 24);
    
    /*load stuff*/

    mouse = gf2d_sprite_load_all("smo/images/pointer.png",32, 32, 16);
	penguin = gf2d_sprite_load_all("smo/images/1797.png", 41, 42, 8);
	sb = gf2d_sprite_load_all("smo/images/skateboard.png", 40, 13, 5);
	coinSpr = gf2d_sprite_load_all("smo/images/coin.png", 74, 74, 3);

	/*initialize stuff*/

	initEntityManager(&entityManager);
	
	player = initPlayer(player, &entityManager);
	player->animList = getAnimListFromFile("smo/anim/penguin.anim");
	player->sprite = penguin;
	player->frames = 64;
	setBounds(player);

	initSkateboard(skateboard, &entityManager);
	skateboard->ent->animList = getAnimListFromFile("smo/anim/skateboard.anim");
	skateboard->ent->sprite = sb;
	skateboard->ent->frames = 15;

	size_t c;
	Entity *currCoin = coins; //coin iterator
	for (c = 0; c < MAX_COINS; c++)
	{
		currCoin = initCoin(currCoin, &entityManager, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		currCoin->animList = getAnimListFromFile("smo/anim/coin.anim");
		currCoin->sprite = coinSpr;
		currCoin->frames = 8;
		setEntityAnim(currCoin, "up");
		setBounds(currCoin);
		currCoin++;
	}

	initAudio();
	loadMusic("smo/audio/music.ogg");
	playMusic();

	lvlList = getLevelListFromFile("smo/level/level.lvl");
	loadLevelFile(&lvlList, "smo/level/level.lvl", &entityManager); //loads backgrounds, bgm, and obstacle data
	//slog("numLevels %d", lvlList.numLevels);

	background = lvlList.levels[0].background;
	loadLevel(&lvlList, currLevel, background, &entityManager);
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
		SDL_PumpEvents(); //update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); //get the keyboard state for this frame

		//update mouse anim frame
		mf += 0.1f;
		if (mf >= 16.0)
		{
			mf = 0;
		}

		if (state == 0) /*MAIN MENU*/
		{
			gf2d_graphics_clear_screen();

			for (i = 0; i < 2; i++)
			{
				gf2d_sprite_draw_image(background, backgroundPos[i]);
				scrollUp(&backgroundPos[i].y, background, PLAYER_SPEED, NULL, NULL);
			}

			drawText
			(font, textSurface, "Start",
				0, 0,
				0, 0, 0,
				255, 255, 255);

			drawText
			(font, textSurface, "Exit",
				0, 30,
				0, 0, 0,
				255, 255, 255);

			drawText
			(font, textSurface,
				"SMO: Skateboarder Mudflap Ollie",
				0, SCREEN_HEIGHT_HALF,
				0, 0, 0,
				255, 255, 255);

			drawText
			(font, textSurface,
				"WASD/Arrows: move, Space: jump, Q: speed up (100 pt. cost), E: superjump (100 pt. cost)",
				0, SCREEN_HEIGHT_HALF + 30,
				0, 0, 0,
				255, 255, 255);

			if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(1))
			{
				if (!clicking)
				{
					p.x = mx;
					p.y = my;
					if (SDL_PointInRect(&p, &rStart))
					{
						player->alive = 1;
						state++;
					}
					else if (SDL_PointInRect(&p, &rExit))
					{
						done = 1;
					}
					clicking = 1;
				}
			}
			else
			{
				clicking = 0;
			}
		}
		else if (state == 1) /*GAME*/
		{

			/*UPDATE*/

			if (keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP])
			{
				if (!typing)
				{
					jump(player);
					typing = 1;
				}
			}
			else if (keys[SDL_SCANCODE_Q])
			{
				if (!typing)
				{
					if (currScore >= 100)
					{
						player->upgrade = 1;
						player->delay = SDL_GetTicks() + 10000;
						updateScore(-100);
						typing = 1;
					}
				}
			}
			else if (keys[SDL_SCANCODE_E])
			{
				if (!typing)
				{
					if (currScore >= 100)
					{
						player->upgrade = 2;
						jump(player);
						updateScore(-100);
						typing = 1;
					}
				}
			}
			else if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
			{
				if (!typing)
				{
					turn(&player->direction, 0);
					typing = 1;
				}
			}
			else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
			{
				if (!typing)
				{
					turn(&player->direction, 1);
					typing = 1;
				}
			}
			else
			{
				typing = 0;
			}

			if (player->alive && currLevel < 2)
			{
				if (SDL_GetTicks() > (1 + currLevel) * 30000)
				{
					currLevel++;
					loadLevel(&lvlList, currLevel, background, &entityManager);
				}
			}

			if (player->upgrade == 1 && player->delay < SDL_GetTicks())
				player->upgrade = 0;

			updateAllEntities(&entityManager);

			/*DRAW*/

			gf2d_graphics_clear_screen(); //clears drawing buffers

			//all drawing should happen between clear_screen and next_frame

			//backgrounds drawn first
			for (i = 0; i < 2; i++)
			{
				gf2d_sprite_draw_image(background, backgroundPos[i]);

				if (player->upgrade == 1)
					scrollUp(&backgroundPos[i].y, background, PLAYER_SPEED_MOD, NULL, NULL);
				else
					scrollUp(&backgroundPos[i].y, background, PLAYER_SPEED, NULL, NULL);
			}

			//entities next
			drawAllEntities(&entityManager);

			//UI elements last
			snprintf(scoreStr, sizeof(scoreStr), "Points: %d", currScore);
			drawText
			(font, textSurface, scoreStr,
				0, 0,
				0, 0, 0,
				255, 255, 255);

			drawText
			(font, textSurface, "Exit",
				0, 30,
				0, 0, 0,
				255, 255, 255);

			//exit button
			if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(1))
			{
				if (!clicking)
				{
					p.x = mx;
					p.y = my;
					if (SDL_PointInRect(&p, &rExit))
					{
						currScore = 0;
						state--;
					}
					clicking = 1;
				}
			}
			else
			{
				clicking = 0;
			}
		}

		gf2d_sprite_draw(
			mouse,
			vector2d(mx, my),
			NULL,
			NULL,
			NULL,
			NULL,
			&mouseColor,
			(int)mf);

        gf2d_graphics_next_frame(); //render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; //exit condition

        //printf("Rendering at %f FPS\n",gf2d_graphics_get_frames_per_second());
    }

	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

/*---END MAIN---*/
