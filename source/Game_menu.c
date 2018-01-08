/*
 * Game_menu.c
 *
 * Description in Game_menu.h
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */
#include "Game_menu.h"

/*
 * This function reads the state of the game of the previous games and
 * stored in a text file called /GameState.txt.
 */
bool readGameState()
{
	//Open the file in read mode
	FILE* file = fopen("/GameState.txt","r");
	if(file != NULL)
	{
		//Read the value and put it into the global variables
		fscanf(file,"%i %i %i %i %i %i\n",&min, &sec, &state, &bg_h, &bg_v, &door_unlocked);
		// close the file
		fclose(file);
		return true;
	}else
	{
		return false;
	}
}

/*
 * This function writes the state of the game
 * into the text file /GameState.txt.
 */
bool writeGameState()
{
	//Open the file in write mode to overwrite
	FILE* file = fopen("/GameState.txt","w+");
	if(file != NULL)
	{
		//Print the value in the file
		fprintf(file,"%i %i %i %i %i %i\n", min, sec, state, bg_h, bg_v, door_unlocked);
		//Close the file
		fclose(file);
		return true;
	}else
	{
		return false;
	}
}
