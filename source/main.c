#include <nds.h>
#include <stdio.h>
#include "Graphics.h"
#include "irq_management.h"
#include "Game.h"

//------------------------------------------------------------------------------
int main(void) {
//------------------------------------------------------------------------------
	typedef enum{MenuStart, Room1, Room2, MenuEnd}State;
	State state = MenuStart;
	bool is_solved = false;

	while(true) {
		switch(state){
			case MenuStart:{
				configure_MenuStart_gfx();
				is_solved = play_MenuStart();

				if(is_solved) state = Room1;
				break;
			}
			case Room1:{
				configure_room1_gfx();
				configure_room1_irq();
				is_solved = play_Room1();

				if(is_solved) state = Room2;
				break;
			}
			case Room2:{
				configure_Room2();
				is_solved = play_Room2();

				if(is_solved) state = MenuEnd;
				break;
			}
			case MenuEnd:{
				configure_MenuEnd();
				is_solved = play_MenuEnd();

				if(is_solved) state = MenuStart;
				break;
			}
		}	// End of switch

		swiWaitForVBlank();
	}	// End of while

	return 0;

}	// End of main
