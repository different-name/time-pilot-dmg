#include "main.h"

#include "../res/bullet_tiles.h"
#include "../res/ship_tiles.h"
#include "../res/sky_map.h"
#include "../res/sky_tiles.h"
#include "calc.h"

Player player;
int8_t joypad_state;

Enemy enemies[ENEMY_COUNT];

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
	set_sprite_data(SHIP_SPRITE_INDEX, MAX_ROTATION, ship_tiles);
	set_sprite_tile(SHIP_SPRITE_INDEX, 0);
	player.gameObject.position.x = PLAYER_X;
	player.gameObject.position.y = PLAYER_Y;
	move_sprite(SHIP_SPRITE_INDEX, PLAYER_X, PLAYER_Y);

	for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
		set_sprite_tile(SHIP_SPRITE_INDEX + 1 + i, 0);
		set_sprite_prop(SHIP_SPRITE_INDEX + 1 + i, S_PALETTE);
		enemies[i].gameObject.position.x = (i + 1) * 30;
		enemies[i].gameObject.position.y = (i + 1) * 30;
		move_sprite(SHIP_SPRITE_INDEX + 1 + i, enemies[i].gameObject.position.x,
					enemies[i].gameObject.position.y);
	}

	// Turn the background map, sprites and display on
	SHOW_BKG;
	SHOW_SPRITES;
	DISPLAY_ON;
}

void game_loop(void) {
	update_player_rotation();
	update_player_position();

	for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
		update_enemy_rotation(i);
		update_enemy_position(i);
	}
}

void update_player_rotation(void) {
	// Only update player rotation every n frames
	if (player.rotation_counter < SHIP_ROTATION_THRESHOLD) {
		player.rotation_counter += 1;
		return;
	}

	// Retrieve current direction the dpad is held in, -1 represents no input
	int8_t target_direction = get_dpad_direction();
	if (target_direction == -1) return;

	// The sprite_index represents the current rotation of the player
	player.gameObject.sprite_index =
			step_to_rotation(player.gameObject.sprite_index, target_direction);
	set_sprite_tile(SHIP_SPRITE_INDEX, player.gameObject.sprite_index);

	// Reset frame counter
	player.rotation_counter = 0;
}

int8_t get_dpad_direction(void) {
	joypad_state = joypad();

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

// Scroll the world
void update_player_position(void) {
	player.gameObject.velocity =
			velocity_from_rotation_and_distance(player.gameObject.sprite_index, 255);

	int16_t scroll_x = player.gameObject.velocity.x + player.pixel_offset.x; // Subpixels to scroll
																			 // by
	player.pixel_offset.x = scroll_x % 255; // Subpixel offset remainder
	scroll_x /= 255;						// Whole pixels

	int16_t scroll_y = player.gameObject.velocity.y + player.pixel_offset.y; // Subpixels to scroll
																			 // by
	player.pixel_offset.y = scroll_y % 255; // Subpixel offset remainder
	scroll_y /= 255;						// Whole pixels

	// Averaging x and y movement if travelling diagonally, reduces stair effect
	if (player.gameObject.sprite_index % QUARTER_ROTATION == 2) {
		int16_t average = (abs(player.pixel_offset.x) + abs(player.pixel_offset.y)) / 2;
		// Maintain direction of movement
		player.pixel_offset.x = player.pixel_offset.x < 0 ? -average : average;
		player.pixel_offset.y = player.pixel_offset.y < 0 ? -average : average;
	}

	scroll_bkg(scroll_x, scroll_y);
}

void update_enemy_rotation(uint8_t index) {
	// Only update enemy rotation every n frames
	if (enemies[index].rotation_counter < SHIP_ROTATION_THRESHOLD) {
		enemies[index].rotation_counter += 1;
		return;
	}

	// Retrieve current direction the enemy is in
	int8_t target_direction =
			direction_to_point(enemies[index].gameObject.position, player.gameObject.position);

	// The sprite_index represents the current rotation of the enemy
	enemies[index].gameObject.sprite_index =
			step_to_rotation(enemies[index].gameObject.sprite_index, target_direction);
	set_sprite_tile(SHIP_SPRITE_INDEX + 1 + index, enemies[index].gameObject.sprite_index);

	// Reset frame counter
	enemies[index].rotation_counter = 0;
}

void update_enemy_position(uint8_t index) {
	enemies[index].gameObject.velocity =
			velocity_from_rotation_and_distance(enemies[index].gameObject.sprite_index, 255);

	enemies[index].gameObject.position.x += enemies[index].gameObject.velocity.x;
	enemies[index].gameObject.position.y += enemies[index].gameObject.velocity.y;

	move_sprite(index + 1, enemies[index].gameObject.position.x / 255,
				enemies[index].gameObject.position.y / 255);

	// int16_t scroll_x = player.gameObject.velocity.x + player.pixel_offset.x; // Subpixels to
	// scroll
	// 																		 // by
	// player.pixel_offset.x = scroll_x % 255; // Subpixel offset remainder
	// scroll_x /= 255;						// Whole pixels

	// int16_t scroll_y = player.gameObject.velocity.y + player.pixel_offset.y; // Subpixels to
	// scroll
	// 																		 // by
	// player.pixel_offset.y = scroll_y % 255; // Subpixel offset remainder
	// scroll_y /= 255;						// Whole pixels

	// // Averaging x and y movement if travelling diagonally, reduces stair effect
	// if (player.gameObject.sprite_index % QUARTER_ROTATION == 2) {
	// 	int16_t average = (abs(player.pixel_offset.x) + abs(player.pixel_offset.y)) / 2;
	// 	// Maintain direction of movement
	// 	player.pixel_offset.x = player.pixel_offset.x < 0 ? -average : average;
	// 	player.pixel_offset.y = player.pixel_offset.y < 0 ? -average : average;
	// }
}