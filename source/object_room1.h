/*
 * object_room1.h
 *
 *  Created on: Dec 23, 2017
 *      Author: nds
 */

#ifndef OBJECT_ROOM1_H_
#define OBJECT_ROOM1_H_

#define NUM_OF_OBJECT 5

typedef enum{hotpot, radio, card, door, trap, none}object_type;

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
