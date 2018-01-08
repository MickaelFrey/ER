/*
 * Game.c
 *
 * Description in Game.h
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#include "Game.h"

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
	//Code to exit the room
	int code_door[5] = {0, 7, 7, 3, 11};
	int code_trap[5] = {3, 8, 10, 4, 2};

	//Declare objects that can be touched
	struct Object obj[NUM_OF_OBJECT];
	object_type obj_touched = none;
	configure_objects(obj);

	//Declare a touchPosition variable
	touchPosition touch;

	//See the map's sprite
	bool hide_sprite = false;

	//Speed when scrolling the map of the sub engine
	int view_speed = 5;

	//Init the sound library
	mmInitDefaultMem((mm_addr)soundbank_bin);
	//Load module
	mmLoad(MOD_GAME);
	//Load effect
	mmLoadEffect(SFX_MORSECODE);
	//Start sound of the game
	mmStart(MOD_GAME,MM_PLAY_LOOP);

	bool trap_unlocked = false;

	while(true){
		//Scan the keys and identify which key is held
		scanKeys();

		u16 keys = keysHeld();

		//Read the touchscreen
		touchRead(&touch);

		//Assign shift registers (they are not readable)
		REG_BG0HOFS_SUB = bg_h;
		REG_BG0VOFS_SUB = bg_v;

		if(door_unlocked){
			//Update local variables that track the shifting
			if(keys & KEY_DOWN){
				//Shifting vertically from down to up
				bg_v += view_speed;
			}
			if(keys & KEY_LEFT){
				//Shifting horizontally from left to right
				bg_h -= view_speed;
			}
			if(keys & KEY_UP){
				//Shifting vertically from up to down
				bg_v -= view_speed;
			}
			if(keys & KEY_RIGHT){
				//Shifting horizontally from right to left
				bg_h += view_speed;
			}
		}
		//Limit the shift according to the size of the background
		if(bg_h < 0) 	bg_h = 0;
		if(bg_h > 255)	bg_h = 255;
		if(bg_v < 0) 	bg_v = 0;
		if(bg_v > 319)	bg_v = 319;

		//The Key X hide the additional information pop-up in the screens
		if(keys & KEY_X ){
			hide_sprite = false;
			reset_room1_gfx();
		}
		if(keys & KEY_START){
			return false;
		}

		//Identify if an object is touched and run the corresponding function
		if(touch.px | touch.py){
			obj_touched = object_touched(obj, touch.px + bg_h, touch.py + bg_v);
			switch(obj_touched){
				case first_msg: {
					hide_sprite = true;
					display_first_msg();
					break;
				}
				case stars: {
					hide_sprite = true;
					display_stars();
					break;
				}
				case hotpot: {
					play_hotpot();
					break;
				}
				case radio: {
					play_radio();
					break;
				}
				case card: {
					hide_sprite = true;
					display_morse();
					break;
				}
				case door: {
					if(!door_unlocked){
						door_unlocked = play_locker(code_door);
						reset_room1_gfx();
					}
					break;
				}
				case trap: {
					if(!trap_unlocked){
						trap_unlocked = play_locker(code_trap);
						reset_room1_gfx();
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
    		hide_sprite,	// Hide this sprite
    		false, false,	// Horizontal or vertical flip
    		false			// Mosaic
    		);
    	//Update the sprites
		oamUpdate(&oamMain);

		//Wait until the screen refresh
		swiWaitForVBlank();

		if(trap_unlocked){
			return true; //Return true; the room is solved
		}
	}
}

/*
 * Define the game corresponding to MenuEnd
 */
void play_MenuEnd(){
	int i, j, k, number, timer[5];

	timer[0] = min / 10;
	timer[1] = min % 10;
	timer[2] = 12;
	timer[3] = sec / 10;
	timer[4] = sec % 10;

	for(i = 0; i < 5; i++){				//For the 5 slots of the timer
		number = timer[i];
		for(j = 0; j < 6; j++)			//For the six tiles height of the digits
			for(k = 0; k < 4; k++)		//For the four tiles width of the digits
				BG_MAP_RAM(10)[(j + 9)*32 + k + 8+i*4] = (u16)(j*4+k)+24*number;
	}

	//Wait until the screen refresh in order to avoid tiring
	swiWaitForVBlank();
}
