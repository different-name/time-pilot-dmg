#ifndef MAIN_H
#define MAIN_H

#include <gb/gb.h>
#include <stdbool.h>
#include <stdio.h>

#define SHIP_ROTATION_THRESHOLD 3

#define PLAYER_X 84
#define PLAYER_Y 80

#define SHIP_SPRITE_INDEX 0

#define ENEMY_COUNT 3

#define MAX_ROTATION 16
// For some reason, MAX_ROTATION / x has to be cast to a signed integer
#define HALF_ROTATION (int8_t)(MAX_ROTATION / 2)
#define QUARTER_ROTATION (int8_t)(MAX_ROTATION / 4)

typedef struct {
	uint8_t x;
	uint8_t y;
} Point2D;

typedef Point2D Counter2D;

typedef struct {
	Point2D position;
	uint8_t sprite_index;
} GameObject;

typedef struct {
	GameObject gameObject;
	uint8_t rotation_counter;
	Counter2D movement_counter;
} Ship;

void init_gfx(void);
void game_loop(void);

void update_player_rotation(void);
uint8_t get_dpad_direction(void);

void update_player_position(void);

void update_enemy_rotation(uint8_t index);

void update_enemy_position(uint8_t index);

#endif