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
};

int object_touched(const struct Object *obj, int x, int y){
	int i;
	int object_num = -1;

	for(i=0;i<NUM_OF_OBJECT;i++){
		if(obj[i].border_left <= x && \
				obj[i].border_right >= x && \
				obj[i].border_up <= y &&\
				obj[i].border_down 	>= y){

			object_num = i;
			break;
		}
	}
	return object_num;
};

