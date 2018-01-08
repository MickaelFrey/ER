/*
 * object_room1.h
 *
 * Manage the objects that can be touched in the game.
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#ifndef OBJECT_ROOM1_H_
#define OBJECT_ROOM1_H_

#include <nds.h>

#define NUM_OF_OBJECT 7

typedef enum{first_msg, stars, hotpot, radio, card, door, trap, none}object_type;

struct Object{
	object_type which_object;
	int border_left;
	int border_right;
	int border_up;
	int border_down;
};

void configure_objects(struct Object *obj);

object_type object_touched(const struct Object *obj, int x, int y);

#endif /* OBJECT_ROOM1_H_ */
