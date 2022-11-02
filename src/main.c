#include <gb/gb.h>
#include <stdbool.h>
#include "../res/sky_map.h"
#include "../res/sky_tiles.h"
#include "../res/ship_tiles.h"

void init_gfx() {
    // Load background tiles and then map
    set_bkg_data(0, 19u, sky_tiles);
    set_bkg_tiles(0, 0, sky_mapWidth, sky_mapHeight, sky_mapPLN0);

	// Load ship
	OBP0_REG = 0xE0; // Updating sprite color palette to [black, dark grey, white, white] https://gbdev.gg8.se/forums/viewtopic.php?id=230
	set_sprite_data(0, 16u, ship_tiles);
	set_sprite_tile(0, 0);
	set_sprite_palette();

	// Turn the background map, sprites and display on
    SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;
}

void main(void)
{
	init_gfx();

	// Move ship to center of screen
	move_sprite(0, 84, 88);

	// Tracking player movement
	uint8_t direction = 0;
	bool up = true;
	bool left = false;
	uint16_t vCounterX = 0;
	uint16_t vCounterY = 0;
	uint16_t directionCounter = 0;
	uint8_t velocities[8] = { 0, 98, 180, 236, 255, 236, 180, 98 }; // Used to get velocity at certain angles, precalculated

    // Game main loop processing goes here
    while (1) {
		uint8_t targetDirection = direction;

		// Set target direction, one of 8 values, 16 directions total (due to smoothing)
		if (joypad() & J_UP && joypad() & J_RIGHT) {
			targetDirection = 2;
		} else if (joypad() & J_RIGHT && joypad() & J_DOWN) {
			targetDirection = 6;
		} else if (joypad() & J_DOWN && joypad() & J_LEFT) {
			targetDirection = 10;
		} else if (joypad() & J_LEFT && joypad() & J_UP) {
			targetDirection = 14;
		} else if (joypad() & J_UP) {
			targetDirection = 0;
		} else if (joypad() & J_RIGHT) {
			targetDirection = 4;
		} else if (joypad() & J_DOWN) {
			targetDirection = 8;
		} else if (joypad() & J_LEFT) {
			targetDirection = 12;
		}

		// Turn one direction at a time, with minimum 4 frames between each change
		if (directionCounter >= 3) { // 4 frame requirement
			// Calculate which direction to turn
			if (targetDirection > direction) {
				directionCounter = 0; // Reset frame counter
				if ((targetDirection - direction + 16) % 16 < 8) { // Turn clockwise
					direction = (direction + 1) % 16;
				} else { // Turn anticlockwise
					if (direction == 0) {
						direction = 15;
					} else {
						direction = (direction - 1) % 16;
					}
				}
				set_sprite_tile(0, direction);
			} else if (targetDirection < direction) {
				directionCounter = 0; // Reset frame counter
				if ((direction - targetDirection + 16) % 16 < 8) { // Turn anticlockwise
					if (direction == 0) {
						direction = 15;
					} else {
						direction = (direction - 1) % 16;
					}
				} else { // Turn clockwise
					direction = (direction + 1) % 16;
				}
				set_sprite_tile(0, direction);
			}
		} else if (directionCounter < 3) {
			directionCounter++; // Incriment frame counter
		}

		// Velocity does not effect direction, must track inverted movement
		if (12 < direction || direction < 4) {
			up = true;
		} else {
			up = false;
		}
		if (8 < direction) {
			left = true;
		} else {
			left = false;
		}

		// Incriment movement		
		if (direction == 2 || direction == 6 || direction == 10 || direction == 14) {
			vCounterX += velocities[direction % 8];
			vCounterY = vCounterX;
		} else {
			vCounterX += velocities[direction % 8];
			vCounterY += velocities[(direction + 4) % 8];
		}

		// Move ship by 1 pixel if needed
		int8_t scrollX = 0;
		int8_t scrollY = 0;
		
		if (vCounterX >= 256) {
			if (left) {
				scrollX = -1;
			} else {
				scrollX = 1;
			}
			vCounterX -= 256;
		}
		
		if (vCounterY >= 256) {
			if (up) {
				scrollY = -1;
			} else {
				scrollY = 1;
			}
			vCounterY -= 256;
		}

		scroll_bkg(scrollX, scrollY);

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}
