#include <SDL.h>
#include <stdio.h>

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "entity_manager.h"
#include "physics.h"
#include "player.h"
#include "animation.h"


//#define SCREEN_HEIGHT_HALF 360
//#define SCREEN_HEIGHT_1POINT5 1080
//#define SCREEN_HEIGHT_2X 1440

/*----------------*/
/*---BEGIN MAIN---*/
/*----------------*/

int main(int argc, char *argv[])
{

	/*SPRITES*/

	//Uint32 *time;
	Sprite *background;
	Vector2D backgroundPos[2];
	Sprite *mouse;
	Sprite *penguin;

	EntityManager entityManager;

	Player *player = (Player*)malloc(sizeof(Player));

	const Uint8 *keys;
	Vector4D mouseColor = { 255,100,255,200 };
	Bool clicking = 0;
	int mx, my; //mouse pos
	float mf = 0; //mouse anim frame [0,16.0)

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

    gf2d_sprite_init(3);

    SDL_ShowCursor(SDL_DISABLE);
    
    /*load stuff*/

	background = gf2d_sprite_load_image("smo/images/backgrounds/street.jpg");
    mouse = gf2d_sprite_load_all("smo/images/pointer.png",32, 32, 16);
	penguin = gf2d_sprite_load_all("smo/images/1797.png", 41, 42, 8);

	/*initialize stuff*/

	backgroundPos[0] = vector2d(0, 0);
	backgroundPos[1] = vector2d(0, SCREEN_HEIGHT);

	initEntityManager(&entityManager);
	
	initPlayer(player, &entityManager);
	player->ent->animList = loadAnimFileToList("smo/anim/penguin.anim");
	player->ent->sprite = penguin;
	player->ent->frames = 64;

    /*main game loop*/

	while (!done)
	{

		/*UPDATE*/

		SDL_PumpEvents(); //update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); //get the keyboard state for this frame
		//*time = SDL_GetTicks();

		//update mouse anim frame
		mf += 0.1f;
		if (mf >= 16.0)
		{
			mf = 0;
		}

		if (SDL_GetMouseState(&mx, &my))
		{
			if (SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				/*if (!clicking)
				{

					if (entityManager.entRef < MAX_ENTITIES-1)
					{
						Entity *ent = initSingleEntity(&entityManager);
						ent->active = 1;
						ent->position = vector2d(mx, my);
						ent->sprite = penguin;
						ent->frames = 64;
					}
				}
				clicking = 1;*/
			}
		}
		else
		{
			//clicking = 0;
		}

		

			if (keys[SDL_SCANCODE_A])
			{
				if (!clicking)
				{
					turn(&player->ent->direction, 0);
					clicking = 1;
				}
			}
			else if (keys[SDL_SCANCODE_D])
			{
				if (!clicking)
				{
					turn(&player->ent->direction, 1);
					clicking = 1;
				}
			}
			else
			{
				clicking = 0;
			}


		/*if (keys[SDL_SCANCODE_BACKSPACE])
		{
			if (entityManager.entRef > 0)
			{
				popEntList(&entityManager);
			}
		}*/

		updateAllEntities(&entityManager);

		/*DRAW*/
        
        gf2d_graphics_clear_screen(); //clears drawing buffers

        //all drawing should happen between clear_screen and next_frame

        //backgrounds drawn first
		for (i = 0; i < 2; i++)
		{
			gf2d_sprite_draw_image(background, backgroundPos[i]);
			scrollUp(&backgroundPos[i].y, PLAYER_SPEED, NULL, NULL);
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
