#pragma once

// 상수 설정
#define M_PI 22.f / 7.f

// 맵 관련 설정
#define MAP_CELL_SIZE 1.f

// 카메라 관련 설정
#define CAMERA_ZOOM_LEVEL 20.f

// 월드 관련 설정
#define WORLD_GRAVITY_X 0.f
#define WORLD_GRAVITY_Y 28.f
#define WORLD_VELOCITY_ITERATION 8
#define WORLD_POSITION_ITERATION 3

// 플레이어 관련 설정
#define PLAYER_MOVE_VELOCITY 7.f
#define PLAYER_JUMP_VELOCITY -12.f
#define PLAYER_NORMALIZED_HEIGHT 1.28f
#define PLAYER_SENSOR_HX 0.4f
#define PLAYER_SENSOR_HY 0.01f
#define PLAYER_MAX_JUMP_COUNT 2
#define PLAYER_INVINCIBILITY_TIME 1.f

// 적 관련 설정
#define ENEMY_MOVE_VELOCITY 5.f
#define ENEMY_RADIUS 0.5f
#define ENEMY_DESTROY_TIME 2.f
#define ENEMY_STOP_TIME_AFTER_ATTACKED 1.f

// 오브젝트 관련 설정
#define OBJECT_COIN_SIZE 0.8f

// UI 관련 설정
#define UI_VIEW_SIZE 100.f
#define UI_CHARACTER_SCALE 0.1f

// 스킬 관련 설정
#define SKILL_MELEE_ATTACK_COOLDOWN 0.f
#define SKILL_MELEE_ATTACK_DAMAGE 1.f
#define SKILL_MELEE_ATTACK_SKILLTIME 0.5f