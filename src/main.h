#ifndef MAIN_H
#define MAIN_H

#include <gb/gb.h>
#include <stdbool.h>
#include <stdio.h>

#define SHIP_ROTATION_THRESHOLD 2
#define ENEMY_COUNT 4
#define PLAYER_BULLET_COUNT 6
#define ENEMY_BULLET_COUNT 5

#define PLAYER_X 84
#define PLAYER_Y 80

#define COOLDOWN_FRAMES 8

#define PLAYER_SPRITE_INDEX 0
#define ENEMY_SPRITE_INDEX (PLAYER_SPRITE_INDEX + 1)
#define BULLET_SPRITE_INDEX (ENEMY_SPRITE_INDEX + ENEMY_COUNT)

#define MIN_POS_X 1
#define MAX_POS_X 167
#define MIN_POS_Y 8
#define MAX_POS_Y 159

typedef struct {
	int8_t x;
	int8_t y;
} Vector8;

typedef struct {
	uint8_t x;
	uint8_t y;
} UVector8;

typedef struct {
	uint16_t x;
	uint16_t y;
} UVector16;

typedef struct {
	UVector8 position;
	uint8_t rotation;
	UVector16 movement_counter;
	uint8_t sprite_index;
	bool enabled;
} GameObject;

typedef struct {
	GameObject gameObject;
	uint8_t rotation_counter;
	uint8_t fire_cooldown;
} Player;

void init_gfx(void);
void game_loop(void);

void update_player_rotation(void);
uint8_t get_dpad_direction(void);
void update_player_position(void);

void update_enemy_rotation(GameObject* enemy);
void update_enemy_position(GameObject* enemy);

void handle_player_firing(void);
void spawn_bullet(GameObject* ship, GameObject bullets[], bool friendly);
void update_bullet_position(GameObject* bullet);

#endif