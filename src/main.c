#include "main.h"

#include "../res/bullet_tiles.h"
#include "../res/ship_tiles.h"
#include "../res/sky_map.h"
#include "../res/sky_tiles.h"
#include "calc.h"

Ship player;
uint8_t joypad_state = 0;
uint8_t last_joypad_state = 0;

uint8_t frame_counter = 0;
bool paused = false;

Ship enemies[ENEMY_COUNT];

Vector8 world_movement;

void main(void) {
	init_gfx();

	while (1) {
		joypad_state = joypad();

		if ((joypad_state & J_START) && !(last_joypad_state & J_START)) {
			paused = !paused;
		}

		if (!paused) {
			game_loop();
		}

		last_joypad_state = joypad_state;
		frame_counter++;

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
	player.gameObject.position.x = PLAYER_X;
	player.gameObject.position.y = PLAYER_Y;
	move_sprite(PLAYER_SPRITE_INDEX, PLAYER_X, PLAYER_Y);

	for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
		set_sprite_tile(ENEMY_SPRITE_INDEX + i, 0);
		set_sprite_prop(ENEMY_SPRITE_INDEX + i, S_PALETTE);
		enemies[i].gameObject.position.x = (i + 1) * 30;
		enemies[i].gameObject.position.y = (i + 1) * 30;
		enemies[i].movement_counter.x = i;
		enemies[i].movement_counter.y = i;
		move_sprite(ENEMY_SPRITE_INDEX + i, enemies[i].gameObject.position.x,
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
		if ((frame_counter + i) % 3 == 0) { // Update rotation every 3rd frame
			update_enemy_rotation(i);
		} else { // Update position every frame except third
			update_enemy_position(i);
		}
	}
}

void update_player_rotation(void) {
	// Only update player rotation every n frames
	if (player.rotation_counter < SHIP_ROTATION_THRESHOLD) {
		player.rotation_counter++;
		return;
	}

	// Retrieve current direction the dpad is held in, -1 represents no input
	int8_t target_direction = get_dpad_direction();
	if (target_direction == MAX_ROTATION) return; // Out of range if dpad not pressed

	// The sprite_index represents the current rotation of the player
	player.gameObject.rotation = step_to_rotation(player.gameObject.rotation, target_direction);
	set_sprite_tile(PLAYER_SPRITE_INDEX, player.gameObject.rotation);

	// Reset frame counter
	player.rotation_counter = 0;
}

uint8_t get_dpad_direction(void) {
	if (joypad_state & J_UP) {
		if (joypad_state & J_LEFT) return MAX_ROTATION / 8 * 7;
		if (joypad_state & J_RIGHT) return MAX_ROTATION / 8 * 1;
		return 0;
	} else if (joypad_state & J_DOWN) {
		if (joypad_state & J_LEFT) return MAX_ROTATION / 8 * 5;
		if (joypad_state & J_RIGHT) return MAX_ROTATION / 8 * 3;
		return MAX_ROTATION / 8 * 4;
	} else if (joypad_state & J_LEFT) {
		return MAX_ROTATION / 8 * 6;
	} else if (joypad_state & J_RIGHT) {
		return MAX_ROTATION / 8 * 2;
	}

	return MAX_ROTATION; // Out of range if dpad not pressed
}

void update_player_position(void) {
	// Scroll background by value calculated last frame
	// This is done because updating enemy positions is slower than updating the background pos
	scroll_bkg(world_movement.x, world_movement.y);

	world_movement = movement_from_velocity(&player);

	for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
		enemies[i].gameObject.position.x -= world_movement.x;
		enemies[i].gameObject.position.y -= world_movement.y;
		move_sprite(ENEMY_SPRITE_INDEX + i, enemies[i].gameObject.position.x,
					enemies[i].gameObject.position.y);
	}
}

void update_enemy_rotation(uint8_t index) {
	UVector8 enemy_position = {0, 0};
	enemy_position = enemies[index].gameObject.position;
	UVector8 player_position = {0, 0};
	player_position = player.gameObject.position;

	// Retrieve current direction the enemy is in
	int8_t target_direction = direction_to_point(enemy_position, player_position);

	// The sprite_index represents the current rotation of the enemy
	enemies[index].gameObject.rotation =
			step_to_rotation(enemies[index].gameObject.rotation, target_direction);
	set_sprite_tile(ENEMY_SPRITE_INDEX + index, enemies[index].gameObject.rotation);
}

void update_enemy_position(uint8_t index) {
	Vector8 enemy_movement = {0, 0};
	enemy_movement = movement_from_velocity(&enemies[index]);
	enemies[index].gameObject.position.x += enemy_movement.x;
	enemies[index].gameObject.position.y += enemy_movement.y;
	move_sprite(ENEMY_SPRITE_INDEX + index, enemies[index].gameObject.position.x,
				enemies[index].gameObject.position.y);
}