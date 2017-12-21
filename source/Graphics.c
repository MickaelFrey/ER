#include <nds.h>
#include "Graphics.h"
#include "background_room1.h"

/*
 * Configure the graphics settings for the MenuStart
 */
void configure_MenuStart(){
	//Enable a proper RAM memory bank for sub engine
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;

	//Configure the sub engine in mode 5 (2D) and activate Backgrounds 2 and 0
	REG_DISPCNT_SUB = MODE_0_2D | DISPLAY_BG0_ACTIVE;
}

/*
 * Configure the graphics settings for the Room1
 */
void configure_room1_gfx(){
	/*
	 * Main engine
	 */

	//Enable a proper RAM memory bank for the main engine
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;

	//Configure the main engine in mode 0 (2D) and activate Backgrounds 3 and 0
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE;

	/*
	 * Sub engine:
	 * background_room1, 64x48 tiles + palette 256 entries (8 bit/px) + map
	 */

	// Activate sub engine and background 0 in tiled mode using 64x64 map
	VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG;
	REG_DISPCNT_SUB  = MODE_5_2D | DISPLAY_BG0_ACTIVE;
	BGCTRL_SUB[0] = BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_64x64;

	// Copy tiles and palette to the corresponding place
	swiCopy(background_room1Tiles, BG_TILE_RAM_SUB(1), background_room1TilesLen/2);
	swiCopy(background_room1Pal, BG_PALETTE_SUB, background_room1PalLen/2);

	// Copy map to the map base(s): As the used map is 3 times a standard one,
	// we will need 3 map bases (i.e. 64x48 components * 16 bits = 6 KB)

	// TOP LEFT quadrant of the image in first map base
	int i;
	for(i=0; i<32; i++)
		dmaCopy(&background_room1Map[i*64], &BG_MAP_RAM_SUB(0)[i*32], 64);

	// TOP RIGHT quadrant of the image in second map base
	//for(i=0; i<32; i++)
		//dmaCopy(&background_room1Map[i*64+32], &BG_MAP_RAM_SUB(1)[i*32], 64);

	// BOTTOM LEFT quadrant of the image in third map base
	//for(i=0; i<32; i++)
		//dmaCopy(&background_room1Map[(i+32)*64], &BG_MAP_RAM_SUB(2)[i*32], 64);

	// BOTTOM RIGHT quadrant of the image in fourth map base
	//for(i=0; i<32; i++)
		//dmaCopy(&background_room1Map[(i+32)*64+32], &BG_MAP_RAM_SUB(3)[i*32], 64);

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
