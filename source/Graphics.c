#include <nds.h>
#include <stdio.h>
#include "Graphics.h"
#include "background_room1.h"
#include "Game_room1.h"
#include "digits.h"
#include "background_room1_main.h"
#include "zone.h"
#include "morse.h"
#include "locker.h"
#include "keyboard.h"
#include "object_room1.h"
#include "start_main.h"
#include "start_sub.h"

/*
 * Configure the graphics settings for the MenuStart
 */
void configure_MenuStart_gfx(){
	/*
	 * Main 2D engine
	 */
	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	//Configure the main engine in mode 5 (2D) and activate BG2
	REG_DISPCNT = MODE_5_2D |  DISPLAY_BG2_ACTIVE;

	BGCTRL[2] = BG_BMP_BASE(0) | BgSize_B16_256x256;

	//Configure affine matrix (no transformation)
	REG_BG2PA = 256;
	REG_BG2PC = 0;
	REG_BG2PB = 0;
	REG_BG2PD = 256;

	swiCopy(start_mainBitmap, BG_GFX, start_mainBitmapLen/2);


	/*
	 * Sub 2D engine:
	 */

	//Enable a proper RAM memory bank for the sub engine
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	//Configure the main engine in mode 5 (2D) and activate BG2
	REG_DISPCNT_SUB = MODE_5_2D |  DISPLAY_BG2_ACTIVE;

	BGCTRL_SUB[2] = BG_BMP_BASE(0) | BgSize_B16_256x256;

	//Configure affine matrix (no transformation)
	REG_BG2PA_SUB = 256;
	REG_BG2PC_SUB = 0;
	REG_BG2PB_SUB = 0;
	REG_BG2PD_SUB = 256;

	swiCopy(start_subBitmap, BG_GFX_SUB, start_mainBitmapLen/2);
}


/*
 * Configure the graphics settings for the Room1
 */
void configure_room1_gfx(){
	/*
	 * Main 2D engine
	 */

	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG;

	//Configure the main engine in mode 5 (2D) and activate BG2 and BG3
	REG_DISPCNT = MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;

	//Configure BG0, BG2 and BG3
	BGCTRL[0] = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(10) | BG_TILE_BASE(0);
	BGCTRL[2] = BG_BMP_BASE(2) | BgSize_B16_256x256;
	BGCTRL[3] = BG_BMP_BASE(8) | BgSize_B16_256x256;

	//Copy the tiles and the palette to the corresonding location
	swiCopy(digitsTiles, BG_TILE_RAM(0), digitsTilesLen);
	swiCopy(digitsPal, BG_PALETTE, digitsPalLen);

	int tile;
	//Make the background transparent with the twelfth part of the digits
	for(tile = 0; tile <1024; tile++)
		BG_MAP_RAM(10)[tile] = 24*12;

    //Affine Marix Transformation for BG2
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;

    //Load background_room1_main in the VRAM of the MAIN
	//swiCopy(background_room1_mainPal, BG_PALETTE, background_room1_mainPalLen/2);
	swiCopy(background_room1_mainBitmap, BG_BMP_RAM(2), background_room1_mainBitmapLen/2);

	//Set up the priority of the backgrounds in order to display BG2
	//BGCTRL[0] = (BGCTRL[0] & 0xFFFC) | 0;
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 0;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 1;

	//Set up memory bank to work in sprite mode (offset since we are using VRAM A for backgrounds)
	VRAM_F_CR = VRAM_ENABLE | VRAM_F_MAIN_SPRITE_0x06400000;

	//Initialize sprite manager and the engine
	oamInit(&oamMain, SpriteMapping_1D_64, false);

	//Allocate space for the graphic to show in the sprite
	gfx = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);

	//Copy data for the graphic (palette and bitmap)
	swiCopy(zonePal, SPRITE_PALETTE, zonePalLen/2);
	swiCopy(zoneTiles, gfx, zoneTilesLen/2);

	/*
	 * Sub 2D engine:
	 * background_room1, 64x64 tiles + palette 256 entries (8 bit/px) + map
	 */

	// Activate sub engine and background 0 in tiled mode using 64x64 map
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	REG_DISPCNT_SUB  = MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG2_ACTIVE;
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x64;
	//BGCTRL_SUB[1] = BG_COLOR_256 | BG_MAP_BASE(4) | BG_TILE_BASE(5) | BG_32x32;
	BGCTRL_SUB[2] = BG_BMP_BASE(5) | BgSize_B8_256x256;

	// Copy tiles and palette to the corresponding place
	swiCopy(background_room1Tiles, BG_TILE_RAM_SUB(1), background_room1TilesLen/2);
	swiCopy(background_room1Pal, BG_PALETTE_SUB, background_room1PalLen/2);

	//Set up the priority of the backgrounds in order to display BG0
	BGCTRL_SUB[0] = (BGCTRL_SUB[0] & 0xFFFC) | 0;
	BGCTRL_SUB[2] = (BGCTRL_SUB[2] & 0xFFFC) | 1;

	// Copy map to the map base(s): As the used map is 4 times a standard one,
	// we will need 4 map bases (i.e. 64x64 components * 16 bits = 8 KB)

	// TOP LEFT quadrant of the image in first map base
	int i;
	for(i=0; i<32; i++)
		dmaCopy(&background_room1Map[i*64], &BG_MAP_RAM_SUB(0)[i*32], 64);
	// TOP RIGHT quadrant of the image in second map base
	for(i=0; i<32; i++)
		dmaCopy(&background_room1Map[i*64+32], &BG_MAP_RAM_SUB(1)[i*32], 64);

	// BOTTOM LEFT quadrant of the image in third map base
	for(i=0; i<32; i++)
		dmaCopy(&background_room1Map[(i+32)*64], &BG_MAP_RAM_SUB(2)[i*32], 64);

	// BOTTOM RIGHT quadrant of the image in fourth map base
	for(i=0; i<32; i++)
		dmaCopy(&background_room1Map[(i+32)*64+32], &BG_MAP_RAM_SUB(3)[i*32], 64);

	// BOTTOM LEFT quadrant of the image in third map base
	//for(i=0; i<32; i++)
		//dmaCopy(&mergedsub_room1Map[(i+64)*64], &BG_MAP_RAM_SUB(4)[i*32], 64);
}

/*
 * Configure the graphics settings in order to display the morse map
 */
void display_morse(){
	/*
	 * As a remind, BG2 is used to display background_room1_main.png on the MAIN.
	 * So, we just have to configure BG3 for morse.png
	 */

    //Affine Marix Transformation
    REG_BG3PA = 256;
	REG_BG3PC = 0;
	REG_BG3PB = 0;
	REG_BG3PD = 256;

	swiCopy(morseBitmap, BG_BMP_RAM(8), morseBitmapLen/2);

	//Set up the priority of the backgrounds in order to display BG3
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 1;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 0;
}

/*
 * Configure the graphics settings needed when we want to enter once we believe
 * we found the good code in order to go to the next step.
 */
void display_locker(){
	/*
	 * As a remind, BG2 is used to display background_room1_main.png on the MAIN.
	 * So, we just have to configure BG3 for locker.png
	 */
	/*
	 * Main engine
	 */

	//Affine Marix Transformation
	REG_BG3PA = 256;
	REG_BG3PC = 0;
	REG_BG3PB = 0;
	REG_BG3PD = 256;

	swiCopy(lockerBitmap, BG_BMP_RAM(8), lockerBitmapLen/2);

	//Set up the priority of the backgrounds in order to display BG3
	BGCTRL[2] = (BGCTRL[2] & 0xFFFC) | 1;
	BGCTRL[3] = (BGCTRL[3] & 0xFFFC) | 0;

	/*
	 * Sub engine
	 */

	//Affine Marix Transformation

	REG_BG2PA_SUB = 256;
	REG_BG2PC_SUB = 0;
	REG_BG2PB_SUB = 0;
	REG_BG2PD_SUB = 256;

	swiCopy(keyboardBitmap, BG_BMP_RAM_SUB(5), keyboardBitmapLen/2);
	swiCopy(keyboardPal, BG_PALETTE_SUB, keyboardPalLen/2);

	//Set up the priority of the backgrounds in order to display BG2
	BGCTRL_SUB[0] = (BGCTRL_SUB[0] & 0xFFFC) | 1;
	BGCTRL_SUB[2] = (BGCTRL_SUB[2] & 0xFFFC) | 0;
}

/*
 * Display the digits on the locker already displayed
 */
void display_digits(int locker[], Check check){
	int i, j, k, number;

	for(i = 0; i < 5; i++){				//For the 5 slots of the locker
		number = locker[i];
		for(j = 0; j < 6; j++)			//For the six tiles height of the digits
			for(k = 0; k < 4; k++)		//For the four tiles width of the digits
				BG_MAP_RAM(10)[(j + 9)*32 + k + 2+i*6] = (u16)(j*4+k)+24*number;
	}

	if(check == unknown) BG_PALETTE[1] = ARGB16(1,0,0,0);
	if(check == correct) BG_PALETTE[1] = ARGB16(1,0,31,0);
	if(check == wrong) BG_PALETTE[1] = ARGB16(1,31,0,0);
}

/*
 * Configure the graphics settings for the Room2
 */
void configure_Room2(){
	//...
}

/*
 * Configure the graphics settings for the MenuEnd
 */
void configure_MenuEnd(){
	//...
}

