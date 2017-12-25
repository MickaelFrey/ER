#include <nds.h>
#include <stdio.h>
#include "Game.h"
#include "object_room1.h"
#include "Game_room1.h"
#include "Graphics.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"


/*
 * Define the game corresponding to MenuStart
 */
int play_MenuStart(){

	return 0;
}

/*
 * Define the game corresponding to Room1
 */
int play_Room1(){

	// Declare background shift variable
	int  bg_h=0, bg_v=0;
	int x, y;
	int view_speed = 5;
	int i;
	struct Object obj[NUM_OF_OBJECT];
	bool add_display = false;

	configure_objects(obj);

	//SOUND
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load effect
	mmLoadEffect(SFX_MORSECODE);

	while(true){
		//Scan the keys
		scanKeys();

		//Identify which key was pressed and print it in the console
		u16 keys = keysHeld();

		// Assign shift registers (they are not readable!)
		REG_BG0HOFS_SUB = bg_h;
		REG_BG0VOFS_SUB = bg_v;

		//Update local variables that track the shifting
		if(keys & KEY_DOWN){
			//shifting vertically from down to up
			bg_v+=view_speed;
		}
		if(keys & KEY_LEFT){
			//shifting horizontally from left to right
			bg_h-=view_speed;
		}
		if(keys & KEY_UP){
			//shifting vertically from up to down
			bg_v-=view_speed;
		}
		if(keys & KEY_RIGHT){
			//shifting horizontally from right to left
			bg_h+=view_speed;
		}
		if(keys & KEY_A){
			add_display = false;
			//Set up the priority of the background
			BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 0;
			BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 1;
		}

		// Limit the shift according to the size of the background
		if(bg_h < 0) 	bg_h = 0;
		if(bg_h > 255)	bg_h = 255;
		if(bg_v < 0) 	bg_v = 0;
		if(bg_v > 319)	bg_v = 319;


		//Declare a touchPosition variable
		touchPosition touch;
		//Read the touchscreen
		touchRead(&touch);
		//Identify a valid touched coordinates and print them
		if(touch.px | touch.py){
			i = object_touched(obj, touch.px+bg_h, touch.py+bg_v);
			switch(obj[i].which_object){
				case radio: {
					play_radio();
					break;
				}
				case card: {
					add_display = true;
					play_card();
					break;
				}
				default: break;
			}

		}

		x = 65+bg_h/4;
		y = 24+bg_v/4;
    	oamSet(&oamMain, 	// oam handler
    		0,				// Number of sprite
    		x, y,			// Coordinates
    		0,				// Priority
    		0,				// Palette to use
    		SpriteSize_64x64,			// Sprite size
    		SpriteColorFormat_256Color,	// Color format
    		gfx,			// Loaded graphic to display
    		-1,				// Affine rotation to use (-1 none)
    		false,			// Double size if rotating
    		add_display,	// Hide this sprite
    		false, false,	// Horizontal or vertical flip
    		false			// Mosaic
    		);
    	//Update the sprites
		oamUpdate(&oamMain);

		swiWaitForVBlank();
	}

	return 0; // Return zero when room is solved
}

/*
 * Define the game corresponding to Room2
 */
int play_Room2(){

	return 0;
}

/*
 * Define the game corresponding to MenuEnd
 */
int play_MenuEnd(){

	return 0;
}
