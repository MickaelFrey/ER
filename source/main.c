/*
 * main.c
 *
 * Main file for the Escape Room game.
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#include "Game.h"

/*
 * Global variables that store all necessary variables to describe the
 * state of the game
 */
int min = 0, sec = 0, bg_h = 0, bg_v = 0, door_unlocked = 0;
State state = MenuStart;

//------------------------------------------------------------------------------
int main(void) {
//------------------------------------------------------------------------------
	bool is_solved = false, is_new_game = false;

	//Declare a touchPosition variable
	touchPosition touch;

	//Initialize the fat library
	fatInitDefault();

	while(true) {
		switch(state){
			case MenuStart:{
				//Configure graphics for the first menu
				configure_MenuStart_gfx();
				/*
				 * Check if the player want to initialize a new game.
				 * If the player press "continue" all the state of
				 * the previous game are loaded from the file
				 * GameState.txt to continue in this state.
				 */
				is_new_game = play_MenuStart();
				if(is_new_game){
					//Start game initialization if New game
					state = Room1;
					min = 0, sec = 0;
					door_unlocked = 0;
					//Pixel shifts for the initial position of the background
					bg_h = 255;
					bg_v = 64;
				}
				break;
			}
			case Room1:{
				//Configure graphics and IRQ for Room 1
				configure_room1_gfx();
				configure_room1_irq();
				/*
				 * Play the room: blocking function until the player press start
				 * or until the room is solved.
				 */
				is_solved = play_Room1();
				//Save time and game states when exiting the room
				irqDisable(IRQ_TIMER1);
				writeGameState();
				if(is_solved){
					//If room solved go to Menu end with new graphics
					state=MenuEnd;
					configure_MenuEnd_gfx();
					//Show time used to solve the room
					play_MenuEnd();
				}else{
					//If player press start button return to start menu
					state = MenuStart;
				}
				break;
			}
			case MenuEnd:{
				//Wait for the player to touch the EXIT button
				touchRead(&touch);
				if((touch.px > 75) && (touch.px < 174)){
					if((touch.py > 128) && (touch.py < 157)){
						state = MenuStart;
					}
				}
				//Wait that the touchscreen is untouched
				while(touch.px | touch.py){
					touchRead(&touch);
				}
				break;
			}
			default: break;
		}	//End of switch

		swiWaitForVBlank();
	}	//End of while

	return 0;

}	//End of main
