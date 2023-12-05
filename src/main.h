#ifndef MAIN_H
#define MAIN_H

#include <gb/gb.h>
#include <stdbool.h>
#include <stdio.h>

#define SHIP_ROTATION_THRESHOLD 3
#define ENEMY_COUNT 4
#define PLAYER_BULLET_COUNT 5
#define ENEMY_BULLET_COUNT 5

#define PLAYER_X 84
#define PLAYER_Y 80

#define PLAYER_SPRITE_INDEX 0
#define ENEMY_SPRITE_INDEX (PLAYER_SPRITE_INDEX + 1)
#define BULLET_SPRITE_INDEX (ENEMY_SPRITE_INDEX + ENEMY_COUNT)

#define MAX_ROTATION 16

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
	bool enabled;
} GameObject;

typedef struct {
	GameObject gameObject;
	uint8_t rotation_counter;
	UVector16 movement_counter;
} Ship;

void init_gfx(void);
void game_loop(void);

void update_player_rotation(void);
uint8_t get_dpad_direction(void);

void update_player_position(void);

void update_enemy_rotation(uint8_t index);

void update_enemy_position(uint8_t index);

#endif