#ifndef MAIN_H
#define MAIN_H

#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_BULLETS 6
#define MAX_ENEMIES 5

#define PLAYER_SPRITE_INDEX 0
#define ENEMY_SPRITE_INDEX 1
#define BULLET_SPRITE_INDEX 6

#define DECIMAL_FACTOR 10

typedef struct _Pos {
	uint8_t x;
	uint8_t y;
} Pos;

typedef struct _Bullet {
	Pos pos;
	bool ready;
	bool friendly;
} Bullet;

typedef struct _Player {
	Pos pos;
	uint16_t vCounterX;
	uint16_t vCounterY;
	uint8_t direction;
	uint8_t directionCounter;
	Bullet bullets[MAX_BULLETS];
} Player;

// enum behaviour { Straight, Target, Wander }

typedef struct _Enemy {
	Pos pos;
	uint16_t vCounterX;
	uint16_t vCounterY;
	uint8_t direction;
	uint8_t directionCounter;
	Bullet bullets[MAX_BULLETS];
	// uint8_t behaviourTime;
	// uint8_t wanderDirection;
} Enemy;

void init_gfx();
void game_loop();

void handle_player_movement();
void handle_enemy_movement();

void handle_bullet_movement();

uint8_t get_turn_direction(uint8_t start, uint8_t target);
Pos world_to_screen_pos(Pos pos);



void handle_shoot();

#endif