#include <nds.h>
#include <stdio.h>
#include "Game.h"
#include "object_room1.h"
#include "Game_room1.h"
#include "Graphics.h"
#include "irq_management.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"


/*
 * Define the game corresponding to MenuStart
 */
bool play_MenuStart(){
	//SOUND
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_START);

	//Declare a touchPosition variable
	touchPosition touch;

	/*MUSIC STARTS*/
	mmStart(MOD_START,MM_PLAY_LOOP);

	while(true){
		//Read the touchscreen
		touchRead(&touch);
		if(touch.px > 78 && touch.px < 179){
			if(touch.py > 49 && touch.py < 79){
				// Stop music and start new game
				mmStop();
				return true;
			}
			if(touch.py > 109 && touch.py < 139){
				// Continue
				// Stop music and continue last backup if there is one

				mmStop();
				/* TODO
				 * implement a data storage when exiting the game
				 * and use this data here to start the game
				 * The data could be: Time, which room are solving..
				 */
				return false;
			}

		}
	}
}

/*
 * Define the game corresponding to Room1
 */
bool play_Room1(){

	// Declare background shift variable
	int  bg_h=0, bg_v=0;
	int view_speed = 5;
	struct Object obj[NUM_OF_OBJECT];

	//Declare a touchPosition variable
	touchPosition touch;

	bool add_display = false;

	object_type obj_touched = none;

	configure_objects(obj);

	//SOUND
	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_GAME);
	//Load effect
	mmLoadEffect(SFX_MORSECODE);
	/*MUSIC STARTS*/
	mmStart(MOD_GAME,MM_PLAY_LOOP);

	//Reset min, sec, msec and reset the timer that count the play time
	min=0;	sec=0;	msec=0;
	irqEnable(IRQ_TIMER1);

	while(true){
		//Scan the keys and identify which key is held
		scanKeys();
		u16 keys = keysHeld();

		//Read the touchscreen
		touchRead(&touch);

		// Assign shift registers (they are not readable)
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
		// Limit the shift according to the size of the background
		if(bg_h < 0) 	bg_h = 0;
		if(bg_h > 255)	bg_h = 255;
		if(bg_v < 0) 	bg_v = 0;
		if(bg_v > 319)	bg_v = 319;

		//The Key A hide the additionnal information pop-up in the main screen
		if(keys & KEY_A){
			add_display = false;
			//Hide BG2 and show BG3
			BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 0;
			BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 1;
		}

		//Identify if an object is touched and run the corresponding function
		if(touch.px | touch.py){
			obj_touched = object_touched(obj, touch.px+bg_h, touch.py+bg_v);
			switch(obj_touched){
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

		//Update the square that follow the movement of the main screen
    	oamSet(&oamMain, 	// oam handler
    		0,				// Number of sprite
    		65+bg_h/4,		// x Coordinates (4 time slower than the shift)
    		24+bg_v/4,		// y Coordinates
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

		//Wait until the screen refresh
		swiWaitForVBlank();
	}

	return true; // Return true; the room is solved
}

/*
 * Define the game corresponding to Room2
 */
bool play_Room2(){

	return true;
}

/*
 * Define the game corresponding to MenuEnd
 */
bool play_MenuEnd(){

	return true;
}
