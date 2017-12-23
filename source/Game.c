#include <nds.h>
#include <stdio.h>
#include "Game.h"
#include "object_room1.h"
#include "Game_room1.h"
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
	int  bg_h, bg_v;
	int view_speed = 10;
	int i;
	struct Object obj[NUM_OF_OBJECT];

	configure_objects(obj);

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
			if(i>=0 && i<NUM_OF_OBJECT){
				switch(obj[i].which_object){
					case radio: {
						// Shift the image when the radio is touched, DEBUG !!
						bg_h+=view_speed;
						play_radio();
						break;
					}
					case card: {
						// Shift the image when the card is touched, DEBUG !!
						bg_h+=view_speed;
						play_card();
						break;
					}
					default: break;
				}
			}
		}
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
