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

#define MORSE_CODE_LENGTH_SEC	4

/*
 *
 */
void rotateImage_main_BG2(int x, int y, float angle_rads)
{

	// Compute the distance from rotation point to system origin
	float r = sqrt(x*x + y*y);

	// Determine the rotation angle alpha of the image
	float alpha = atan((float)x/(float)y)+angle_rads;

	// Image rotation matrix
	REG_BG2PA = cos(angle_rads)*256;
	REG_BG2PB = sin(angle_rads)*256;
	REG_BG2PC = -sin(angle_rads)*256;
	REG_BG2PD = cos(angle_rads)*256;

	// Image translation
	REG_BG2X = (x-r*sin(alpha))*256-(0<<8);
	REG_BG2Y = (y-r*cos(alpha))*256-(0<<8);

}
/*
 *
 */
void rotateImage_main_BG3(int x, int y, float angle_rads)
{

	// Compute the distance from rotation point to system origin
	float r = sqrt(x*x + y*y);

	// Determine the rotation angle alpha of the image
	float alpha = atan((float)x/(float)y)+angle_rads;

	// Image rotation matrix
	REG_BG3PA = cos(angle_rads)*256;
	REG_BG3PB = sin(angle_rads)*256;
	REG_BG3PC = -sin(angle_rads)*256;
	REG_BG3PD = cos(angle_rads)*256;

	// Image translation
	REG_BG3X = (x-r*sin(alpha))*256-(64<<8);
	REG_BG3Y = (y-r*cos(alpha))*256-(32<<8);

}


void play_hotpot(){
	display_hotpot();

	float i=0;
	float j=0;
	while(true){
		//Scan the keys and identify which key is held
		scanKeys();
		u16 keys = keysHeld();

		if((keys & KEY_X)){	//Quit the locker-mode by pressing on X
			rotateImage_main_BG3(128, 96,i);
			i=i+0.01;
		}
		if((keys & KEY_A)){	//Quit the locker-mode by pressing on X
			rotateImage_main_BG2(128, 96,j);
			j=j+0.01;
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
	display_morse();		//Display the morse-map
}

bool play_locker(int code[5]){
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

	//Initialize the locker-array with the digit-reference 12, which means a full transparent group of tiles (6x4)
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
					slot_locker++;	//Increase the index in order to save
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

			if(keypressed == 14){	//In order to check if the code is the right one
				i = 0;
				while(i < 5){
					if(locker[i] == code[i]){
						if(i == 4) check = correct;
						i++;
					}
					else{
						check = wrong;
						break;
					}
				}
			}

			//Display the pressed key(s) on the locker and paint it(them) in accordance to the state of check (unknown -> black; true -> green; wrong -> red)
			display_digits(locker, check);

			//Avoid multi-filling of the locker by waiting that the touchscreen is untouched
			while(touch.px | touch.py){
				touchRead(&touch);
			}

		}	//End of fill-in the locker and display it

		if((keys & KEY_X) || (check == correct)){	//Quit the locker-mode by pressing on X
			int i;
			for(i = 0; i < 5; i++) locker[i] = 12;	//Re-initialize the locker as empty
			slot_locker = 0;
			display_digits(locker, check);
			oamEnable(&oamMain);					//Re-enable the sprite
			break;									//Break the while-loop
		}

	}	//End of while-loop

	if(check == correct){
		return true;		//The right code has been found
	}
	else{
		return false;		//The code is still unknown
	}
}
