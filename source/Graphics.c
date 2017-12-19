#include <nds.h>
#include "Graphics.h"

/*
 * Configure the graphics settings for the MenuStart
 */
void configure_MenuStart(){
	//...
}

/*
 * Configure the graphics settings for the Room1
 */
void configure_Room1(){
	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG; //Bank for the main engine

	//Configure the main engine in mode 0 (2D) and activate Backgrounds 3 and 0
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE;

	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;


	//Configure the sub engine in mode 5 (2D) and activate Backgrounds 2 and 0
	REG_DISPCNT_SUB = MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG0_ACTIVE;
}

/*
 * Configure the graphics settings for the Room2
 */
void configure_Room2(){
	//...
}

/*
 * Configure the graphics settings for the MenuEnd
 */
void configure_MenuEnd(){
	//...
}
