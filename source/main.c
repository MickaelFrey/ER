#include <nds.h>
#include <stdio.h>
#include "Graphics.h"
#include "Game.h"

// Parameters
//#define ...

// Variables
typedef enum{MenuStart, Room1, Room2, MenuEnd}state;
int is_solved = 0;

//------------------------------------------------------------------------------
int main(void) {
//------------------------------------------------------------------------------

	state state = MenuStart;

	while(true) {
		switch(state){
		case MenuStart:{
			configure_MenuStart();
			is_solved = play_MenuStart();

			if(is_solved == 1) state = Room1;
			break;
		}
		case Room1:{
			configure_room1_gfx();
			is_solved = play_Room1();

			if(is_solved == 1) state = Room2;
			break;
		}
		case Room2:{
			configure_Room2();
			is_solved = play_Room2();

			if(is_solved == 1) state = MenuEnd;
			break;
		}
		case MenuEnd:{
			configure_MenuEnd();
			is_solved = play_MenuEnd();

			if(is_solved == 1) state = MenuStart;
			break;
		}

		}	// End of switch
		is_solved = 0;

		swiWaitForVBlank();
	}	// End of while

	return 0;

}	// End of main
