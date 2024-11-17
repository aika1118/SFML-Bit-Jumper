#pragma once

// 상수 설정
#define M_PI 22.f / 7.f

// 맵 관련 설정
#define MAP_CELL_SIZE 1.f

// 카메라 관련 설정
#define CAMERA_ZOOM_LEVEL 20.f

// 월드 관련 설정
#define WORLD_GRAVITY_X 0.f
#define WORLD_GRAVITY_Y 17.f
#define WORLD_VELOCITY_ITERATION 8
#define WORLD_POSITION_ITERATION 3

// 플레이어 관련 설정
#define PLAYER_MOVE_VELOCITY 7.f
#define PLAYER_JUMP_VELOCITY -12.f
#define PLAYER_NORMALIZED_HEIGHT 1.28f
#define PLAYER_SENSOR_HX 0.3f
#define PLAYER_SENSOR_HY 0.1f
#define PLAYER_MAX_JUMP_COUNT 2