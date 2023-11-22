#include "main.h"

#include "../res/bullet_tiles.h"
#include "../res/ship_tiles.h"
#include "../res/sky_map.h"
#include "../res/sky_tiles.h"
#include "calc.h"

Player player;

// Velocity lookup table, precalculated to save resources
// Calculated using sin(n * 22.5 deg) * 255 where n = {0, 1, 2, 3, 4, 3, 2, 1};
const uint8_t velocities[8] = {0, 98, 180, 236, 255, 236, 180, 98};

void main(void) {
	init_gfx();

	while (1) {
		game_loop();
		// Done processing, yield CPU and wait for start of next frame
		wait_vbl_done();
	}
}

void init_gfx(void) {
	// Load background tiles and then map
	set_bkg_data(0, 19u, sky_tiles);
	set_bkg_tiles(0, 0, sky_mapWidth, sky_mapHeight, sky_mapPLN0);

	// Load ship
	OBP0_REG = 0xE0; // Updating sprite color palette to
					 // [black, dark grey, white, white]
					 // https://gbdev.gg8.se/forums/viewtopic.php?id=230
	OBP1_REG = 0x2C;
	set_sprite_data(PLAYER_SPRITE_INDEX, MAX_ROTATION, ship_tiles);
	set_sprite_tile(PLAYER_SPRITE_INDEX, 0);
	player.gameobject.position.x = PLAYER_X;
	player.gameobject.position.y = PLAYER_Y;
	move_sprite(PLAYER_SPRITE_INDEX, player.gameobject.position.x, player.gameobject.position.y);

	// Turn the background map, sprites and display on
	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;
}

void game_loop(void) {
	update_player_rotation();
	update_player_position();
}

void update_player_rotation(void) {
	// Only update player rotation every n frames
	if (player.rotation_counter < PLAYER_ROTATION_THRESHOLD) {
		player.rotation_counter += 1;
		return;
	}

	// Retrieve current direction the dpad is held in, -1 represents no input
	int8_t target_direction = get_dpad_direction();
	if (target_direction == -1) return;

	// The sprite_index represents the current rotation of the player
	player.gameobject.sprite_index =
			step_to_rotation(player.gameobject.sprite_index, target_direction);
	set_sprite_tile(PLAYER_SPRITE_INDEX, player.gameobject.sprite_index);

	// Reset frame counter
	player.rotation_counter = 0;
}

int8_t get_dpad_direction(void) {
	int8_t joypad_state = joypad();

	if (joypad_state & J_UP) {
		if (joypad_state & J_LEFT) return 14;
		if (joypad_state & J_RIGHT) return 2;
		return 0;
	} else if (joypad_state & J_DOWN) {
		if (joypad_state & J_LEFT) return 10;
		if (joypad_state & J_RIGHT) return 6;
		return 8;
	} else if (joypad_state & J_LEFT) {
		return 12;
	} else if (joypad_state & J_RIGHT) {
		return 4;
	}

	return -1;
}

// Increments / decrements rotation by 1 toward target rotation
int8_t step_to_rotation(int8_t r1, int8_t r2) {
	if (r1 == r2) return r1; // Do not step if rotations match

	// Calculate direction to step
	int8_t direction = ((r2 - r1 + MAX_ROTATION) % MAX_ROTATION <= HALF_ROTATION) ? 1 : -1;

	return (r1 + direction + MAX_ROTATION) % MAX_ROTATION; // Step toward target rotation
}

// Scroll the world
void update_player_position(void) {
	player.gameobject.velocity =
			velocity_from_rotation_and_distance(player.gameobject.sprite_index, 255);

	int16_t move_x = (player.gameobject.velocity.x + player.remainder_x) / 255;
	player.remainder_x = (player.gameobject.velocity.x + player.remainder_x) % 255;

	int16_t move_y = (player.gameobject.velocity.y + player.remainder_y) / 255;
	player.remainder_y = (player.gameobject.velocity.y + player.remainder_y) % 255;

	if (player.gameobject.sprite_index % QUARTER_ROTATION == 2) {
		int16_t abs_x = player.remainder_x < 0 ? -player.remainder_x : player.remainder_x;
		int16_t abs_y = player.remainder_y < 0 ? -player.remainder_y : player.remainder_y;
		int16_t average = (abs_x + abs_y) / 2;
		player.remainder_x = player.remainder_x < 0 ? -average : average;
		player.remainder_y = player.remainder_y < 0 ? -average : average;
	}

	scroll_bkg(move_x, move_y);
}

Vector2D velocity_from_rotation_and_distance(int8_t rotation, uint16_t distance) {
	Vector2D velocity;

	velocity.x = velocities[rotation % HALF_ROTATION]; // Use velocity lookup table
	velocity.x = (velocity.x * distance) / 255;		   // How many subpixels to travel
	velocity.x *= rotation > HALF_ROTATION ? -1 : 1;   // Invert direction if needed

	velocity.y = velocities[(rotation + QUARTER_ROTATION) % HALF_ROTATION];
	velocity.y = (velocity.y * distance) / 255;
	velocity.y *= rotation > QUARTER_ROTATION && rotation < (QUARTER_ROTATION * 3) ? 1 : -1;

	return velocity;
}

//  // Used
// to get velocity at certain angles, precalculated

// Player player;
// Enemy enemiesMAX_ENEMIES];

// Pos screenPos;
// int8_t scrollX = 0;
// int8_t scrollY = 0;
// uint8_t joypadPrevious = 0;

// uint8_t tempCount = 0;

// void main() {
// 	init_gfx();

// 	set_sprite_tile(ENEMY_SPRITE_INDEX, 0);
// 	set_sprite_prop(ENEMY_SPRITE_INDEX, S_PALETTE);
// 	enemies[0].pos.x = 103;
// 	enemies[0].pos.y = 62;

//     while (1) {
// 		game_loop();
// 		// Done processing, yield CPU and wait for start of next frame
// 		wait_vbl_done();
//     }
// }

// void init_gfx() {
//     // Load background tiles and then map
//     set_bkg_data(0, 19u, sky_tiles);
//     set_bkg_tiles(0, 0, sky_mapWidth, sky_mapHeight, sky_mapPLN0);

// 	// Load ship
// 	OBP0_REG = 0xE0; // Updating sprite color palette to [black, dark grey,
// white, white] https://gbdev.gg8.se/forums/viewtopic.php?id=230 	OBP1_REG =
// 0x2C; 	set_sprite_data(PLAYER_SPRITE_INDEX, 16u, ship_tiles);
// 	set_sprite_tile(PLAYER_SPRITE_INDEX, 0);
// 	player.pos.x = 84;
// 	player.pos.y = 88;
// 	move_sprite(PLAYER_SPRITE_INDEX, player.pos.x, player.pos.y);

// 	// Turn the background map, sprites and display on
//     SHOW_BKG;
// 	SHOW_SPRITES;
// 	DISPLAY_ON;
// }

// void game_loop() {
// 	handle_player_movement();

// 	handle_bullet_movement();

// 	handle_enemy_movement();

// 	handle_shoot();

// 	joypadPrevious = joypad();

// }

// void handle_player_movement() {
// 	// Scroll by values calculated last frame
// 	scroll_bkg(scrollX, scrollY);
// 	scrollX = 0;
// 	scrollY = 0;

// 	uint8_t targetDirection = player.direction;

// 	// Set target direction, one of 8 values, 16 directions total (due to
// smoothing) 	if (joypad() & J_UP && joypad() & J_RIGHT) {
// targetDirection = 2; 	} else if (joypad() & J_RIGHT && joypad() & J_DOWN)
// { 		targetDirection = 6; 	} else if (joypad() & J_DOWN && joypad() &
// J_LEFT) { 		targetDirection = 10; 	} else if (joypad() & J_LEFT &&
// joypad() & J_UP) { 		targetDirection = 14; 	} else if (joypad() & J_UP)
// { 		targetDirection = 0; 	} else if (joypad() & J_RIGHT) {
// targetDirection = 4; 	} else if (joypad() & J_DOWN) {
// targetDirection = 8; 	} else if (joypad() & J_LEFT) {
// targetDirection = 12;
// 	}

// 	// Change direction by 1 towards target, with minimum 4 frames between each
// change 	if (player.directionCounter >= 3) { 		int8_t modifier =
// get_turn_direction(player.direction, targetDirection);

// 		if (player.direction == 0 && modifier < 0) {
// 			player.direction = 15;
// 		} else {
// 			player.direction = (player.direction + modifier) % 16;
// 		}

// 		set_sprite_tile(PLAYER_SPRITE_INDEX, player.direction);

// 		player.directionCounter = 0;
// 	} else if (player.directionCounter < 3) {
// 		player.directionCounter++;
// 	}

// 	// Incriment movement counters
// 	player.vCounterX += velocities[player.direction % 8];

// 	if ((player.direction - 2) % 4 == 0) { // Sync x and y movement if moving
// diagonally 		player.vCounterY = player.vCounterX; 	} else {
// player.vCounterY += velocities[(player.direction + 4) % 8];
// 	}

// 	if (player.vCounterX >= 255) {
// 		// Invert if moving left at all
// 		if (8 < player.direction) {
// 			scrollX = -1;
// 		} else {
// 			scrollX = 1;
// 		}
// 		player.vCounterX -= 255;
// 	}

// 	if (player.vCounterY >= 255) {
// 		// Invert if moving up at all
// 		if (12 < player.direction || player.direction < 4) {
// 			scrollY = -1;
// 		} else {
// 			scrollY = 1;
// 		}
// 		player.vCounterY -= 255;
// 	}

// 	screenPos.x += scrollX;
// 	screenPos.y += scrollY;
// }

// void handle_enemy_movement() {
// 	if (tempCount == 3) {
// 		tempCount = 0;
// 		return;
// 	} else {
// 		tempCount++;
// 	}

// 	Enemy enemy;
// 	enemy = enemies[0];

// 	Pos enemyScreenPos;
// 	enemyScreenPos = world_to_screen_pos(enemies[0].pos);
// 	uint8_t targetDirection = angle_between_pos(enemyScreenPos, player.pos);

// 	int8_t moveX = 0;
// 	int8_t moveY = 0;

// 	// Change direction by 1 towards target, with minimum 4 frames between each
// change 	if (enemies[0].directionCounter >= 3) { 		int8_t modifier =
// get_turn_direction(enemies[0].direction, targetDirection);

// 		if (enemies[0].direction == 0 && modifier < 0) {
// 			enemies[0].direction = 15;
// 		} else {
// 			enemies[0].direction = (enemies[0].direction + modifier) % 16;
// 		}

// 		set_sprite_tile(ENEMY_SPRITE_INDEX, enemies[0].direction);

// 		enemies[0].directionCounter = 0;
// 	} else if (enemies[0].directionCounter < 3) {
// 		enemies[0].directionCounter++;
// 	}

// 	// Incriment movement counters
// 	enemies[0].vCounterX += velocities[enemies[0].direction % 8];

// 	if ((enemies[0].direction - 2) % 4 == 0) { // Sync x and y movement if
// moving diagonally 		enemies[0].vCounterY = enemies[0].vCounterX; 	}
// else { 		enemies[0].vCounterY += velocities[(enemies[0].direction + 4) %
// 8];
// 	}

// 	if (enemies[0].vCounterX >= 255) {
// 		// Invert if moving left at all
// 		if (8 < enemies[0].direction) {
// 			moveX = -1;
// 		} else {
// 			moveX = 1;
// 		}
// 		enemies[0].vCounterX -= 255;
// 	}

// 	if (enemies[0].vCounterY >= 255) {
// 		// Invert if moving up at all
// 		if (12 < enemies[0].direction || enemies[0].direction < 4) {
// 			moveY = -1;
// 		} else {
// 			moveY = 1;
// 		}
// 		enemies[0].vCounterY -= 255;
// 	}

// 	enemies[0].pos.x += moveX;
// 	enemies[0].pos.y += moveY;

// 	move_sprite(ENEMY_SPRITE_INDEX, enemyScreenPos.x, enemyScreenPos.y);
// 	set_sprite_tile(ENEMY_SPRITE_INDEX, enemies[0].direction);
// }

// void handle_bullet_movement() {

// }

// uint8_t get_turn_direction(uint8_t start, uint8_t target) {
// 	if (start == target) {
// 		return 0;
// 	}

// 	if (start > target) {
// 		start, target = target, start;
// 	}

// 	return ((target - start + 16) % 16 < 8) ? 1 : -1;
// }

// Pos world_to_screen_pos(Pos pos) {
// 	Pos result;
// 	result.x = pos.x - screenPos.x;
// 	result.y = pos.y - screenPos.y;
// 	return result;
// }

// void handle_shoot() {
// 	if (!(joypad() & J_A) || (joypadPrevious & J_A)) {
// 		return;
// 	}
// }