/*
 * Game_menu.h
 *
 * Function to read and write the state of the game in the
 * text file GameState.txt. Return true if the file is correctly open.
 *
 * Variable read/write: min, sec, msec, state, bg_h, bg_v, door_unlocked
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#ifndef BREAK_H_
#define BREAK_H_

#include "Game.h"

/*
 * Read file GameState.txt. Return true if the file is correctly open
 */
bool readGameState();

/*
 * Write file GameState.txt. Return true if the file is correctly open
 */
bool writeGameState();

#endif /* BREAK_H_ */
