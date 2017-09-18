#include <SDL.h>
#include <stdio.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"

#define MAX_ENTITIES 1000

int main(int argc, char *argv[])
{
    /*variable declarations*/

	//Uint32 *time;

	Sprite *background;
	Sprite *mouse;

	//Entity player;
	Entity testEnt[100];
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
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);

    gf2d_graphics_set_frame_delay(16);

    gf2d_sprite_init(1024);

    SDL_ShowCursor(SDL_DISABLE);
    
    /*load stuff*/
	background = gf2d_sprite_load_image("smo/images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("smo/images/pointer.png",32,32,16);

	/*initialize stuff*/
	for (i = 0; i < 100; i++)
	{
		initEntity(&testEnt[i], mouse, NULL, 16);
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
				if (!clicking)
				{

					if (entRef < 100)
					{
						testEnt[entRef].active = 1;
						testEnt[entRef].position = vector2d(mx, my);
						entRef++;
					}
				}
				clicking = 1;
			}
		}
		else
		{
			clicking = 0;
		}
        
        gf2d_graphics_clear_screen(); //clears drawing buffers

        //all drawing should happen between clear_screen and next_frame

        //backgrounds drawn first
        gf2d_sprite_draw_image(background,vector2d(0,0));

		//player.draw(&player);
		for (i = 0; i < 100; i++)
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

        //printf("Rendering at %f FPS\n",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
