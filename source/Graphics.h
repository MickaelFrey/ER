#ifndef GRAPHICS_H_
#define GRAPHICS_H_



#define SCREEN_WIDTH	256
#define	SCREEN_HEIGHT	192

#define	SPRITE_WIDTH	32
#define	SPRITE_HEIGHT	32

//Pointer to the graphic buffer where to store the sprite
u16* gfx;


/*
 * Configure the graphics settings for the MenuStart
 */
void configure_MenuStart_gfx();

/*
 * Configure the graphics settings for the Room1
 */
void configure_room1_gfx();

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
void display_digits(int locker[]);

/*
 * Configure the graphics settings for the Room2
 */
void configure_Room2();

/*
 * Configure the graphics settings for the MenuEnd
 */
void configure_MenuEnd();

#endif /* GRAPHICS_H_ */
