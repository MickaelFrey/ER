#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Game_room1.h"

#define SCREEN_WIDTH	256
#define	SCREEN_HEIGHT	192

#define	SPRITE_WIDTH	32
#define	SPRITE_HEIGHT	32

//Pointer to the graphic buffer where to store the sprite
u16* gfx;

/*
 * Configure the graphics settings in order to display the hot pot
 */
void display_hotpot();
/*
 * Configure the graphics settings for the MenuStart
 */
void configure_MenuStart_gfx();

/*
 * Configure the graphics settings for the Room1
 */
void configure_room1_gfx();

/*
 * reset the graphics settings for the Room1
 */
void reset_room1_gfx();

/*
 * Configure the graphics settings in order to display the first message
 */
void display_first_msg();

/*
 * Configure the graphics settings in order to display the stars picture
 */
void display_stars();

/*
 * Configure the graphics settings in order to display the morse map
 */
void display_morse();

/*
 * Configure the graphics settings needed when we want to enter, once we believe
 * we found the good code, in order to go to the next step.
 */
void display_locker();

/*
 * Display the digits on the locker already displayed
 */
void display_digits(int locker[], Check check);

/*
 * Rotate Image for BG2 the main engine.
 */
void rotateImage_main_BG2(int x, int y, float angle_rads, int tx, int ty);

/*
 * Rotate Image for BG3 the main engine.
 */
void rotateImage_main_BG3(int x, int y, float angle_rads, int tx, int ty);

/*
 * Configure the graphics settings for the MenuEnd
 */
void configure_MenuEnd_gfx();

#endif /* GRAPHICS_H_ */
