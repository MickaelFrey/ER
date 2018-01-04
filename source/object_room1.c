#include <nds.h>
#include "object_room1.h"



void configure_objects(struct Object *obj){
	obj[0].which_object = radio;
	obj[0].border_left = 70;
	obj[0].border_right = 102;
	obj[0].border_up = 326;
	obj[0].border_down = 343;

	obj[1].which_object = card;
	obj[1].border_left = 130;
	obj[1].border_right = 155;
	obj[1].border_up = 197;
	obj[1].border_down = 207;

	obj[2].which_object = door;
	obj[2].border_left = 355;
	obj[2].border_right = 380;
	obj[2].border_up = 210;
	obj[2].border_down = 250;

	obj[3].which_object = trap;
	obj[3].border_left = 350;
	obj[3].border_right = 380;
	obj[3].border_up = 420;
	obj[3].border_down = 450;

	obj[4].which_object = hotpot;
	obj[4].border_left = 234;
	obj[4].border_right = 246;
	obj[4].border_up = 164;
	obj[4].border_down = 170;
};

object_type object_touched(const struct Object *obj, int x, int y){
	int i;

	object_type obj_touched = none;

	for(i=0;i<NUM_OF_OBJECT;i++){
		if(obj[i].border_left <= x && \
				obj[i].border_right >= x && \
				obj[i].border_up <= y &&\
				obj[i].border_down 	>= y){

			obj_touched = obj[i].which_object;
			break;
		}
	}
	return obj_touched;
};

