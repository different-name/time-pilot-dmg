#include "main.h"
#include "calc.h"
#include "../res/sky_map.h"
#include "../res/sky_tiles.h"
#include "../res/ship_tiles.h"
#include "../res/bullet_tiles.h"

const uint8_t velocities[8] = { 0, 98, 180, 236, 255, 236, 180, 98 }; // Used to get velocity at certain angles, precalculated

Player player;
Enemy enemies[MAX_ENEMIES];

Pos screenPos;
int8_t scrollX = 0;
int8_t scrollY = 0;

uint8_t joypadPrevious = 0;

uint8_t tempCount = 0;

void main() {
	init_gfx();

	set_sprite_tile(ENEMY_SPRITE_INDEX, 0);
	set_sprite_prop(ENEMY_SPRITE_INDEX, S_PALETTE);
	enemies[0].pos.x = 103;
	enemies[0].pos.y = 62;



    while (1) {
		game_loop();
		// Done processing, yield CPU and wait for start of next frame
		wait_vbl_done();
    }
}

void init_gfx() {
    // Load background tiles and then map
    set_bkg_data(0, 19u, sky_tiles);
    set_bkg_tiles(0, 0, sky_mapWidth, sky_mapHeight, sky_mapPLN0);

	// Load ship
	OBP0_REG = 0xE0; // Updating sprite color palette to [black, dark grey, white, white] https://gbdev.gg8.se/forums/viewtopic.php?id=230
	OBP1_REG = 0x2C;
	set_sprite_data(PLAYER_SPRITE_INDEX, 16u, ship_tiles);
	set_sprite_tile(PLAYER_SPRITE_INDEX, 0);
	player.pos.x = 84;
	player.pos.y = 88;
	move_sprite(PLAYER_SPRITE_INDEX, player.pos.x, player.pos.y);

	// Turn the background map, sprites and display on
    SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;
}

void game_loop() {
	handle_player_movement();

	handle_bullet_movement();

	handle_enemy_movement();

	handle_shoot();

	joypadPrevious = joypad();

	
}

void handle_player_movement() {
	// Scroll by values calculated last frame
	scroll_bkg(scrollX, scrollY);
	scrollX = 0;
	scrollY = 0;

	uint8_t targetDirection = player.direction;

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

	// Change direction by 1 towards target, with minimum 4 frames between each change
	if (player.directionCounter >= 3) {
		int8_t modifier = get_turn_direction(player.direction, targetDirection);

		if (player.direction == 0 && modifier < 0) {
			player.direction = 15;
		} else {
			player.direction = (player.direction + modifier) % 16;
		}

		set_sprite_tile(PLAYER_SPRITE_INDEX, player.direction);

		player.directionCounter = 0;
	} else if (player.directionCounter < 3) {
		player.directionCounter++;
	}

	// Incriment movement counters
	player.vCounterX += velocities[player.direction % 8];

	if ((player.direction - 2) % 4 == 0) { // Sync x and y movement if moving diagonally
		player.vCounterY = player.vCounterX;
	} else {
		player.vCounterY += velocities[(player.direction + 4) % 8];
	}

	if (player.vCounterX >= 255) {
		// Invert if moving left at all
		if (8 < player.direction) {
			scrollX = -1;
		} else {
			scrollX = 1;
		}
		player.vCounterX -= 255;
	}
	
	if (player.vCounterY >= 255) {
		// Invert if moving up at all
		if (12 < player.direction || player.direction < 4) {
			scrollY = -1;
		} else {
			scrollY = 1;
		}
		player.vCounterY -= 255;
	}

	screenPos.x += scrollX;
	screenPos.y += scrollY;
}

void handle_enemy_movement() {
	if (tempCount == 3) {
		tempCount = 0;
		return;
	} else {
		tempCount++;
	}

	Enemy enemy;
	enemy = enemies[0];

	Pos enemyScreenPos;
	enemyScreenPos = world_to_screen_pos(enemies[0].pos);
	uint8_t targetDirection = angle_between_pos(enemyScreenPos, player.pos);

	int8_t moveX = 0;
	int8_t moveY = 0;

	// Change direction by 1 towards target, with minimum 4 frames between each change
	if (enemies[0].directionCounter >= 3) {
		int8_t modifier = get_turn_direction(enemies[0].direction, targetDirection);

		if (enemies[0].direction == 0 && modifier < 0) {
			enemies[0].direction = 15;
		} else {
			enemies[0].direction = (enemies[0].direction + modifier) % 16;
		}

		set_sprite_tile(ENEMY_SPRITE_INDEX, enemies[0].direction);

		enemies[0].directionCounter = 0;
	} else if (enemies[0].directionCounter < 3) {
		enemies[0].directionCounter++;
	}

	// Incriment movement counters
	enemies[0].vCounterX += velocities[enemies[0].direction % 8];

	if ((enemies[0].direction - 2) % 4 == 0) { // Sync x and y movement if moving diagonally
		enemies[0].vCounterY = enemies[0].vCounterX;
	} else {
		enemies[0].vCounterY += velocities[(enemies[0].direction + 4) % 8];
	}

	if (enemies[0].vCounterX >= 255) {
		// Invert if moving left at all
		if (8 < enemies[0].direction) {
			moveX = -1;
		} else {
			moveX = 1;
		}
		enemies[0].vCounterX -= 255;
	}
	
	if (enemies[0].vCounterY >= 255) {
		// Invert if moving up at all
		if (12 < enemies[0].direction || enemies[0].direction < 4) {
			moveY = -1;
		} else {
			moveY = 1;
		}
		enemies[0].vCounterY -= 255;
	}

	enemies[0].pos.x += moveX;
	enemies[0].pos.y += moveY;

	move_sprite(ENEMY_SPRITE_INDEX, enemyScreenPos.x, enemyScreenPos.y);
	set_sprite_tile(ENEMY_SPRITE_INDEX, enemies[0].direction);
}

void handle_bullet_movement() {

}

uint8_t get_turn_direction(uint8_t start, uint8_t target) {
	if (start == target) {
		return 0;
	}

	if (start > target) {
		start, target = target, start;
	}

	return ((target - start + 16) % 16 < 8) ? 1 : -1;
}

Pos world_to_screen_pos(Pos pos) {
	Pos result;
	result.x = pos.x - screenPos.x;
	result.y = pos.y - screenPos.y;
	return result;
}

void handle_shoot() {
	if (!(joypad() & J_A) || (joypadPrevious & J_A)) {
		return;
	}
}