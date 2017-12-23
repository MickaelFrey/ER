#include <nds.h>
#include <stdio.h>
#include "Game.h"

/*
 * Define the game corresponding to MenuStart
 */
int play_MenuStart(){
	int a = -2, b = 3;
	int c;

	c = a + b;

	return c;
}

/*
 * Define the game corresponding to Room1
 */
int play_Room1(){
	// Declare bg shift variable only once in static (non blocking function)
	static int  bg_h, bg_v;

	int speed = 5;

	//Scan the keys
	scanKeys();

	//Identify which key was pressed and print it in the console
	u16 keys = keysHeld();

	// Assign shift registers (they are not readable!)
	REG_BG0HOFS_SUB = bg_h;
	REG_BG0VOFS_SUB = bg_v;

	//Update local variables that track the shifting
	if(keys & KEY_DOWN){
		//shifting vertically from down to up
		bg_v+=speed;
	}
	if(keys & KEY_LEFT){
		//shifting horizontally from left to right
		bg_h-=speed;;
	}
	if(keys & KEY_UP){
		//shifting vertically from up to down
		bg_v-=speed;;
	}
	if(keys & KEY_RIGHT){
		//shifting horizontally from right to left
		bg_h+=speed;;
	}
	// Limit the shift according to the size of the background
	if(bg_h < 0) 	bg_h = 0;
	if(bg_h > 255)	bg_h = 255;
	if(bg_v < 0) 	bg_v = 0;
	if(bg_v > 319)	bg_v = 319;

	return 0; // Return zero for the moment --> room to solve
}

/*
 * Define the game corresponding to Room2
 */
int play_Room2(){
	int a = -2, b = 3;
	int c;

	c = a + b;

	return c;
}

/*
 * Define the game corresponding to MenuEnd
 */
int play_MenuEnd(){
	int a = -2, b = 3;
	int c;

	c = a + b;

	return c;
}
