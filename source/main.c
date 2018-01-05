#include <nds.h>
#include <stdio.h>
#include "Game_menu.h"
#include "Graphics.h"
#include "irq_management.h"
#include "Game.h"

#include <fat.h>

typedef enum{MenuStart, None, Room1, MenuEnd}State;

/*
 * Global variables that store all necessary variables to describe the
 * state of the game
 */
int min = 0, sec = 0, msec = 0, bg_h=0, bg_v=0;
State state = MenuStart;

//------------------------------------------------------------------------------
int main(void) {
//------------------------------------------------------------------------------
	bool is_solved = false, is_new_game = false;

	//Initialize the fat library
	fatInitDefault();

	while(true) {
		switch(state){
			case MenuStart:{
				configure_MenuStart_gfx();
				is_new_game = play_MenuStart();

				if(is_new_game){
					state = Room1;
					min = 0, sec = 0, msec = 0;
					bg_h=0, bg_v=0;
				}
				break;
			}
			case Room1:{
				configure_room1_gfx();
				configure_room1_irq();
				is_solved = play_Room1();

				if(is_solved){
					state=MenuEnd;
				}else{
					writeGameState();
					state = MenuStart;
				}
				break;
			}
			case MenuEnd:{
				configure_MenuEnd();
				is_solved = play_MenuEnd();

				if(is_solved) state = MenuStart;
				break;
			}
			default: break;
		}	// End of switch

		swiWaitForVBlank();
	}	// End of while

	return 0;

}	// End of main
