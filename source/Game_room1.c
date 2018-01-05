#include <nds.h>
#include <stdio.h>
//#include "math.h"
#include <math.h>

#include "Graphics.h"
#include "Game_room1.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#include "irq_management.h"

#include "background_room1_main.h"

#define MORSE_CODE_LENGTH_SEC	4


void play_hotpot(){
	// Angle_step determine the step of rotation of the carrots in the hot pot
	float angle_step = 0.02;

	// Initialize the rotation of the carrots (center and middle)
	float angle_center = angle_step *111;
	float angle_middle = angle_step *55;

	// Initialize the graphics of the hot pot
	display_hotpot(angle_center, angle_middle);
	while(true){
		//Scan the keys and identify which key is held
		scanKeys();
		u16 keys = keysHeld();
		u16 keys_up = keysUp();

		if((keys & KEY_R)){
			/*
			 * Image in BG3 is the carrots in the center.
			 * The 128x128 picture is rotating around the center of the
			 * Screen;
			 * Center of rotation: x = 256/2 = 128 px, y = 192/2 = 96 px
			 * Shift: tx = (256-128)/2 = 64, ty = (192-128)/2 = 32
			 */
			rotateImage_main_BG3(128, 96,angle_center, 64, 32);
			angle_center+=angle_step;
		}
		if((keys & KEY_L)){
			/*
			 * Image in BG2 is the carrots in the middle.
			 * The 256x192 picture is rotating around the center of the
			 * Screen;
			 * Center of rotation: x = 256/2 = 128 px, y = 192/2 = 96 px
			 * Shift: tx = ty = 0;
			 */
			rotateImage_main_BG2(128, 96,angle_middle, 0, 0);
			angle_middle+=angle_step;
		}
		if(keys_up & KEY_L ||keys_up & KEY_R){
			/*
			 * The rotation is inversed everytime that L1 and R1 are released
			 * to disturb the game
			 */
			angle_step = -angle_step;
		}
		if((keys & KEY_X)){
			/*
			 * Exit the hot pot
			 */
			exit_display_hotpot();
			break;
		}

		//Wait until the screen refresh in order to avoid tiring
		swiWaitForVBlank();
	}
}

void play_radio(){
	static int stored_sec = 0;
	static bool play = true;

	if(play){
		// Start sound effect.
		mmEffect(SFX_MORSECODE);
		stored_sec = min * 60 + sec;
		play = false;
	}
	if(min * 60 + sec - stored_sec > MORSE_CODE_LENGTH_SEC){
		play = true;
	}
}

void play_card(){
	display_morse();
}

bool play_door(){
	Check check = unknown;
	oamDisable(&oamMain);	//Disable the sprite
	display_locker();		//Display the backgrounds for the MAIN and the SUB

	//Declare a touchPosition variable
	touchPosition touch;

	//Wait that the touchscreen is untouched (touchscreen pressed on the door)
	touchRead(&touch);
	while(touch.px | touch.py){
		touchRead(&touch);
	}

	//Code that we have to find
	int code_room1[5] = {3, 7, 10, 4, 2};

	//Initialize the locker-array
	int locker[5];
	int i;
	for(i = 0; i < 5; i++) locker[i] = 12;
	int slot_locker = 0;	//Index of the locker-array

	while(true){
		//Read the touchscreen
		touchRead(&touch);

		//Scan the keys and identify which key is held
		scanKeys();
		u16 keys = keysHeld();

		if(touch.px | touch.py){	//Fill-in the locker
			int keypressed = 12;	//Corresponds to a transparent tile-group
			if((touch.px > 58) && (touch.px < 95)){	//Detect which button is pressed
				if((touch.py > 20) && (touch.py < 45)){
					keypressed = 1;
				}else if((touch.py > 62) && (touch.py < 87)){
					keypressed = 4;
				}else if((touch.py > 104) && (touch.py < 129)){
					keypressed = 7;
				}else if((touch.py > 146) && (touch.py < 170)){
					keypressed = 10; //Pressed "star" button on the keyboard
				}
			}else if((touch.px > 110) && (touch.px < 148)){
				if((touch.py > 20) && (touch.py < 45)){
					keypressed = 2;
				}else if((touch.py > 62) && (touch.py < 87)){
					keypressed = 5;
				}else if((touch.py > 104) && (touch.py < 129)){
					keypressed = 8;
				}else if((touch.py > 146) && (touch.py < 170)){
					keypressed = 0;
				}
			}else if((touch.px > 162) && (touch.px < 199)){
				if((touch.py > 20) && (touch.py < 45)){
					keypressed = 3;
				}else if((touch.py > 62) && (touch.py < 87)){
					keypressed = 6;
				}else if((touch.py > 104) && (touch.py < 129)){
					keypressed = 9;
				}else if((touch.py > 146) && (touch.py < 170)){
					keypressed = 11; //Pressed "grid" button on the keyboard
				}
			}else if((touch.px > 226) && (touch.px < 251)){
				if((touch.py > 62) && (touch.py < 87)){
					keypressed = 13; //Pressed "return" button on the keyboard
				}else if((touch.py > 104) && (touch.py < 129)){
					keypressed = 14; //Pressed "enter" button on the keyboard
				}
			}	//End of the button detection

			if((keypressed >= 0) && (keypressed < 12)){	//Save the pressed key
				if(slot_locker < 5){
					locker[slot_locker] = keypressed;
					slot_locker++;
				}
			}else if(keypressed == 13){		//Clear slot-by-slot the locker
				if(slot_locker > 0){
					locker[slot_locker - 1] = 12;
					slot_locker--;
					check = unknown;
				}
			}

			//Wait until the screen refresh in order to avoid tiring
			swiWaitForVBlank();

			if(keypressed == 14){
				i = 0;
				while(i < 5){
					if(locker[i] == code_room1[i]){
						if(i == 4) check = correct;
						i++;
					}
					else{
						check = wrong;
						break;
					}
				}
			}

			//Display the pressed key on the locker
			display_digits(locker, check);

			//Avoid multi-filling of the locker by waiting that the touchscreen is untouched
			while(touch.px | touch.py){
				touchRead(&touch);
			}

		}	//End of fill-in the locker and display it

		if((keys & KEY_X) || (check == correct)){	//Quit the locker-mode by pressing on X
			int i;
			for(i = 0; i < 5; i++) locker[i] = 12;
			slot_locker = 0;
			display_digits(locker, check);
			oamEnable(&oamMain);
			break;
		}

	}	//End of while-loop

	if(check == correct){
		return true;
	}
	else{
		return false;
	}
}

void play_trap(){
	//Show BG3 in front of BG2
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 1;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 0;
}
