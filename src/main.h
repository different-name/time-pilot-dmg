#ifndef MAIN_H
#define MAIN_H

#include <gb/gb.h>
#include <stdbool.h>
#include <stdio.h>

#define PLAYER_ROTATION_THRESHOLD 3

#define PLAYER_X 84
#define PLAYER_Y 80

#define PLAYER_SPRITE_INDEX 0

#define MAX_ROTATION 16
// For some reason, MAX_ROTATION / x has to be cast to a signed integer
#define HALF_ROTATION (int8_t)(MAX_ROTATION / 2)
#define QUARTER_ROTATION (int8_t)(MAX_ROTATION / 4)

typedef struct {
	uint16_t x;
	uint16_t y;
} Point;

typedef struct {
	int16_t x;
	int16_t y;
} Vector2D;

typedef struct {
	Point position;
	Vector2D velocity;
	uint8_t sprite_index;
} GameObject;

typedef struct {
	GameObject gameobject;
	uint8_t rotation_counter;
	int16_t remainder_x;
	int16_t remainder_y;
} Player;

void init_gfx(void);
void game_loop(void);

void update_player_rotation(void);
int8_t get_dpad_direction(void);
int8_t step_to_rotation(int8_t r1, int8_t r2);

void update_player_position(void);
Vector2D velocity_from_rotation_and_distance(int8_t rotation, uint16_t distance);

// #define MAX_BULLETS 6
// #define MAX_ENEMIES 5
// #define ENEMY_SPRITE_INDEX 1
// #define BULLET_SPRITE_INDEX 6

// #define DECIMAL_FACTOR 10

// typedef struct _Pos {
// 	uint8_t x;
// 	uint8_t y;
// } Pos;

// typedef struct _Bullet {
// 	Pos pos;
// 	bool ready;
// 	bool friendly;
// } Bullet;

// typedef struct _Player {
// 	Pos pos;
// 	uint16_t vCounterX;
// 	uint16_t vCounterY;
// 	uint8_t direction;
// 	uint8_t directionCounter;
// 	Bullet bullets[MAX_BULLETS];
// } Player;

// // enum behaviour { Straight, Target, Wander }

// typedef struct _Enemy {
// 	Pos pos;
// 	uint16_t vCounterX;
// 	uint16_t vCounterY;
// 	uint8_t direction;
// 	uint8_t directionCounter;
// 	Bullet bullets[MAX_BULLETS];
// 	// uint8_t behaviourTime;
// 	// uint8_t wanderDirection;
// } Enemy;

// void init_gfx();
// void game_loop();

// void handle_player_movement();
// void handle_enemy_movement();

// void handle_bullet_movement();

// uint8_t get_turn_direction(uint8_t start, uint8_t target);
// Pos world_to_screen_pos(Pos pos);

// void handle_shoot();

#endif