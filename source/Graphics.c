#include <nds.h>
#include <stdio.h>
#include "Graphics.h"
#include "background_room1.h"
#include "background_room1_main.h"
#include "zone.h"
#include "morse.h"
#include "object_room1.h"

/*
 * Configure the graphics settings for the MenuStart
 */
void configure_MenuStart(){
	//Enable a proper RAM memory bank for sub engine
	//VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	//Configure the sub engine in mode 5 (2D) and activate Backgrounds 2 and 0
	//REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
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
	//VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG;


	//Configure the main engine in mode 5 (2D) and activate BG2 and BG3
	REG_DISPCNT = MODE_5_2D |  DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE;

	//Configure BG 2 and BG3
	BGCTRL[2] = BG_BMP_BASE(0) | BgSize_B8_256x256;
	BGCTRL[3] = BG_BMP_BASE(3) | BgSize_B16_256x256;

    //Affine Marix Transformation
    REG_BG2PA = 256;
    REG_BG2PC = 0;
    REG_BG2PB = 0;
    REG_BG2PD = 256;

	swiCopy(background_room1_mainPal, BG_PALETTE, background_room1_mainPalLen/2);
	swiCopy(background_room1_mainBitmap, BG_GFX, background_room1_mainBitmapLen/2);

	REG_BG3PA = 256;
	REG_BG3PC = 0;
	REG_BG3PB = 0;
	REG_BG3PD = 256;
	REG_BG3X = (1<<8);
	REG_BG3Y = (-20<<8);

	swiCopy(morseBitmap, BG_BMP_RAM(3), morseBitmapLen/2);

	//Set up the priority of the background
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
	 * Main 2D engine
	 */

	// Activate sub engine and background 0 in tiled mode using 64x64 map
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	REG_DISPCNT_SUB  = MODE_5_2D | DISPLAY_BG0_ACTIVE;
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x64;

	// Copy tiles and palette to the corresponding place
	swiCopy(background_room1Tiles, BG_TILE_RAM_SUB(1), background_room1TilesLen/2);
	swiCopy(background_room1Pal, BG_PALETTE_SUB, background_room1PalLen/2);

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

