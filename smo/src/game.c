#include <SDL.h>
#include <stdio.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"

#define MAX_ENTITIES 1000

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
//#define SCREEN_HEIGHT_HALF 360
//#define SCREEN_HEIGHT_1POINT5 1080
//#define SCREEN_HEIGHT_2X 1440

Vector2D scrollUp(Vector2D *pos, Vector2D spd) {
	Vector2D scroll = vector2d(pos->x - spd.x, pos->y - spd.y);
	if( (scroll.x + SCREEN_WIDTH) < 0 ) scroll.x = SCREEN_WIDTH;
	if( (scroll.y + SCREEN_HEIGHT) < 0 ) scroll.y = SCREEN_HEIGHT;
	return scroll;
}

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

	/*ENTITIES*/

	Entity testEnt[MAX_ENTITIES];
	//Entity player;
	int entRef = 0;

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
        "gf2d",
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
    mouse = gf2d_sprite_load_all("smo/images/pointer.png",32,32,16);
	penguin = gf2d_sprite_load_all("smo/images/1797.png", 41, 42, 8);

	/*initialize stuff*/

	backgroundPos[0] = vector2d(0, 0);
	backgroundPos[1] = vector2d(0, SCREEN_HEIGHT);

	for (i = 0; i < MAX_ENTITIES; i++)
	{
		initEntity(&testEnt[i], penguin, NULL, 64);
		testEnt[i].active = 0;
	}

	//initEntity(&player, mouse, NULL, 16);
	//player.position = vector2d(200, 200);

    /*main game loop*/

	while (!done)
	{
		SDL_PumpEvents();   //update SDL's internal event structures
		//*time = SDL_GetTicks();
		keys = SDL_GetKeyboardState(NULL); //get the keyboard state for this frame

		/*update things here*/

		//player.update(&player);

		//update mouse anim frame
		mf += 0.1;
		if (mf >= 16.0)
		{
			mf = 0;
		}

		if (SDL_GetMouseState(&mx, &my))
		{
			if (SDL_BUTTON(SDL_BUTTON_LEFT))
			{
				//if (!clicking)
				//{

					if (entRef < MAX_ENTITIES)
					{
						testEnt[entRef].active = 1;
						testEnt[entRef].position = vector2d(mx, my);
						entRef++;
					}
				//}
				//clicking = 1;
			}
		}
		else
		{
			clicking = 0;
		}
        
        gf2d_graphics_clear_screen(); //clears drawing buffers

        //all drawing should happen between clear_screen and next_frame

        //backgrounds drawn first
		for (i = 0; i < 2; i++)
		{
			gf2d_sprite_draw_image(background, backgroundPos[i]);
			backgroundPos[i] = scrollUp(&backgroundPos[i], vector2d(0, 2));
		}

		//player.draw(&player);
		for (i = 0; i < MAX_ENTITIES; i++)
		{	
			if(testEnt[i].active)
				testEnt[i].draw(&testEnt[i]);
		}
            
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
        
		//if (keys[SDL_SCANCODE_W])
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; //exit condition

        printf("Rendering at %f FPS\n",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

/*---END MAIN---*/
