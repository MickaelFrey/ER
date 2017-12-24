#include <nds.h>
#include <stdio.h>
#include "Game_room1.h"
#include "morse.h"

void play_radio(){

}

void play_card(){

	REG_BG3PA = 256;
	REG_BG3PC = 0;
	REG_BG3PB = 0;
	REG_BG3PD = 256;
	REG_BG3X = (1<<8);
	REG_BG3Y = (-20<<8);

	swiCopy(morseBitmap, BG_BMP_RAM(3), morseBitmapLen/2);

	//Set up the priority of the background
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 1;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 0;

}
