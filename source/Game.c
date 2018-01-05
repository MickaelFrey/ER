#include <nds.h>
#include <stdio.h>
#include "Game.h"
#include "background_room1.h"
#include "background_room1_main.h"
#include "object_room1.h"
#include "Game_room1.h"
#include "Graphics.h"
#include "irq_management.h"
#include "Game_menu.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

extern int min, sec, msec, bg_h, bg_v;

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

	//MUSIC STARTS
	mmStart(MOD_START,MM_PLAY_LOOP);

	while(true){
		//Read the touchscreen
		touchRead(&touch);
		if(touch.px > 78 && touch.px < 179){
			if(touch.py > 49 && touch.py < 79){
				/* New game */
				return true;
			}
			if(touch.py > 109 && touch.py < 139){
				/*
				 * Continue last backup if there is one.
				 * (Data are stored when exiting the game)
				 */
				if(readGameState()){
					return false;
				}

			}

		}
	}
}

/*
 * Define the game corresponding to Room1
 */
bool play_Room1(){

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

	//Pixel shifts for the initial position of the background
	bg_h = 255;
	bg_v = 64;

	//At the beginning, it's impossible to move background_room1

	bool door_unlocked = false; // TRUE FOR THE DEBUG
	bool trap_unlocked = false;

	while(true){
		//Scan the keys and identify which key is held
		scanKeys();


		u16 keys = keysHeld();

		//Read the touchscreen
		touchRead(&touch);

		// Assign shift registers (they are not readable)
		REG_BG0HOFS_SUB = bg_h;
		REG_BG0VOFS_SUB = bg_v;

		if(door_unlocked){
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
		}

		// Limit the shift according to the size of the background
		if(bg_h < 0) 	bg_h = 0;
		if(bg_h > 255)	bg_h = 255;
		if(bg_v < 0) 	bg_v = 0;
		if(bg_v > 319)	bg_v = 319;

		//The Key A hide the additionnal information pop-up in the screens
		if(keys & KEY_X ){
			add_display = false;
			//Hide BG2 and show BG3 for the MAIN engine
			//BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 0;
			//BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 1;

			BGCTRL[0] = (BGCTRL[0] & 0xFFFC) | 0;
			BGCTRL[1] = (BGCTRL[1] & 0xFFFC) | 1;
			BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 2;
			BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 3;

			//Hide BG2 and show BG0 for the SUB engine
			swiCopy(background_room1Pal, BG_PALETTE_SUB, background_room1PalLen/2);
			BGCTRL_SUB[0] = (BGCTRL_SUB[0] & 0xFFFC) | 0;
			BGCTRL_SUB[2] = (BGCTRL_SUB[2] & 0xFFFC) | 1;
		}

		if(keys & KEY_START){
			return false;
		}

		//Identify if an object is touched and run the corresponding function
		if(touch.px | touch.py){
			obj_touched = object_touched(obj, touch.px+bg_h, touch.py+bg_v);
			switch(obj_touched){
				case hotpot: {
					play_hotpot();
					break;
				}
				case radio: {
					play_radio();
					break;
				}
				case card: {
					add_display = true;
					play_card();
					break;
				}
				case door: {
					if(!door_unlocked){
						int code_door[5] = {0, 7, 7, 3, 11};
						door_unlocked = play_locker(code_door);

						// Pallette of the background was overwritten by the carrots BG
						swiCopy(background_room1_mainPal, BG_PALETTE, background_room1_mainPalLen/2);

						//Configure BG0 in tile mode for background_room1_main (don't overlap digits)
						BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(21) | BG_TILE_BASE(2);

						// Assign priority to display BG0
						BGCTRL[0] = (BGCTRL[0] & 0xFFFC) | 0;
						BGCTRL[1] = (BGCTRL[1] & 0xFFFC) | 1;
						BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 2;
						BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 3;

						//Hide BG2 and show BG0 for the SUB engine
						swiCopy(background_room1Pal, BG_PALETTE_SUB, background_room1PalLen/2);
						BGCTRL_SUB[0] = (BGCTRL_SUB[0] & 0xFFFC) | 0;
						BGCTRL_SUB[2] = (BGCTRL_SUB[2] & 0xFFFC) | 1;
					}
					break;
				}
				case trap: {
					if(!trap_unlocked){
						int code_trap[5] = {3, 7, 10, 4, 2};
						trap_unlocked = play_locker(code_trap);

						// Pallette of the background was overwritten by the carrots BG
						swiCopy(background_room1_mainPal, BG_PALETTE, background_room1_mainPalLen/2);

						//Configure BG0 in tile mode for background_room1_main (don't overlap digits)
						BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(21) | BG_TILE_BASE(2);

						// Assign priority to display BG0
						BGCTRL[0] = (BGCTRL[0] & 0xFFFC) | 0;
						BGCTRL[1] = (BGCTRL[1] & 0xFFFC) | 1;
						BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 2;
						BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 3;

						//Hide BG2 and show BG0 for the SUB engine
						swiCopy(background_room1Pal, BG_PALETTE_SUB, background_room1PalLen/2);
						BGCTRL_SUB[0] = (BGCTRL_SUB[0] & 0xFFFC) | 0;
						BGCTRL_SUB[2] = (BGCTRL_SUB[2] & 0xFFFC) | 1;
					}
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

		if(trap_unlocked){
			return true; // Return true; the room is solved
		}
	}
}

/*
 * Define the game corresponding to MenuEnd
 */
bool play_MenuEnd(){

	return true;
}
