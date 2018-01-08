/*
 * Game_room1.h
 *
 * Implementation of all small game that can be played in room 1.
 * Every time an existing object is touched the corresponding play function
 * is called.
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#ifndef GAME_ROOM1_H_
#define GAME_ROOM1_H_

#include <nds.h>
#include <stdio.h>
#include <math.h>
#include "Graphics.h"
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "irq_management.h"
#include "background_room1_main.h"


#define MORSE_CODE_LENGTH_SEC	5

/*
 * Play little game with L1 and R1 button. The purpose is to align three
 * graphics background that turn with L1 and R1 button.
 */
void play_hotpot();
/*
 * Launch morse sound: /-/..../.-././. (T/H/R/E/E)
 */
void play_radio();
/*
 * View picture that help to understand the morse code
 */
void play_card();
/*
 * View of the locker; enter a five digit code to unlock it
 */
bool play_locker(int code[5]);

#endif /* GAME_ROOM1_H_ */
