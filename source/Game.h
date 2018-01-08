/*
 * Game.h
 *
 * Top level function called by main.c to enter in the menu (Start/End)
 * or to play room 1
 *
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#ifndef GAME_H_
#define GAME_H_

#include <nds.h>
#include <stdio.h>
#include "background_room1.h"
#include "background_room1_main.h"
#include "object_room1.h"
#include "Game_room1.h"
#include "Graphics.h"
#include "irq_management.h"
#include "Game_menu.h"
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include <fat.h>

/*
 * Define the possible state of the game
 */
typedef enum{MenuStart, None, Room1, MenuEnd}State;

/*
 * Global variable to describe the game declared in main file
 */
extern int min, sec, bg_h, bg_v, door_unlocked;
extern State state;

/*
 * Define the game corresponding to MenuStart
 */
bool play_MenuStart();

/*
 * Define the game corresponding to Room1
 */
bool play_Room1();

/*
 * Define the game corresponding to MenuEnd
 */
void play_MenuEnd();

#endif /* GAME_H_ */
