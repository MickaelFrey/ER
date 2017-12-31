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
}

void play_trap(){
	//Show BG3 in front of BG2
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 1;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 0;
}
