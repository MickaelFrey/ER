#include <nds.h>
#include <stdio.h>
#include "Graphics.h"
#include "Game_room1.h"

#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#include "irq_management.h"

#define MORSE_CODE_LENGTH_SEC	4

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

void play_door(){
	display_locker();

	//Declare a touchPosition variable
	touchPosition touch;

	//Find which key is pressed on the keyboard
	int locker[5];
	int i;
	for(i = 0; i < 5; i++) locker[i] = 12;
	int slot_locker = 0;

	//while(true){
	//Read the touchscreen
	touchRead(&touch);

	if(touch.px | touch.py){
		int keypressed = 12;
		if((touch.px > 58) && (touch.px < 95)){
			if((touch.py > 20) && (touch.py < 45)){
				keypressed = 1;
			}else if((touch.py > 62) && (touch.py < 87)){
				keypressed = 4;
			}else if((touch.py > 104) && (touch.py < 129)){
				keypressed = 7;
			}else if((touch.py > 146) && (touch.py < 170)){
				keypressed = 10; //pressed on "star" button on the keyboard
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
				keypressed = 11; //pressed on "grid" button on the keyboard
			}
		}else if((touch.px > 226) && (touch.px < 251)){
			if((touch.py > 62) && (touch.py < 87)){
				keypressed = 13; //pressed on "return" button on the keyboard
			}else if((touch.py > 104) && (touch.py < 129)){
				keypressed = 14; //pressed on "enter" button on the keyboard
			}
		}

		if((keypressed >= 0) && (keypressed < 12)){	//Save the pressed key
			locker[slot_locker] = keypressed;
			slot_locker++;
		}else if(keypressed == 13){					//Clear the locker
			int i;
			for(i = 0; i < 5; i++) locker[i] = 12;
			slot_locker = 0;
		}

		//Display the pressed key on the locker
		display_digits(locker);

	//}
	}
}

void play_trap(){
	//Show BG3 in front of BG2
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 1;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 0;
}
