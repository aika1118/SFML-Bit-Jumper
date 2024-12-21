#pragma once

// ��� ����
#define M_PI 22.f / 7.f

// �� ���� ����
#define MAP_CELL_SIZE 1.f
#define MAP_STAGE_1 "./resources/images/stage1.png"
#define MAP_IS_DEBUG_DRAW 0

// ī�޶� ���� ����
#define CAMERA_ZOOM_LEVEL 20.f

// �÷��̾� ���� ����
#define PLAYER_SIZE_RATIO 1.28f
#define PLAYER_SIZE_WIDTH 0.5f
#define PLAYER_SIZE_HEIGHT (PLAYER_SIZE_WIDTH * PLAYER_SIZE_RATIO)
#define PLAYER_SENSOR_HX (PLAYER_SIZE_WIDTH / 2.f) * 0.8f
#define PLAYER_SENSOR_HY 0.01f
#define PLAYER_MAX_JUMP_COUNT 1
#define PLAYER_INVINCIBILITY_TIME 1.f 


// ���� ���� ����
#define WORLD_GRAVITY_X 0.f
#define WORLD_VELOCITY_ITERATION 8
#define WORLD_POSITION_ITERATION 3

// �÷��̾� ���� ���� ����
#define PLAYER_JUMP_HEIGHT 2.f
#define PLAYER_JUMP_TIME 0.5f
// ������
#define WORLD_GRAVITY_Y (8.0f * PLAYER_JUMP_HEIGHT) / (PLAYER_JUMP_TIME * PLAYER_JUMP_TIME) 
// ������
#define PLAYER_JUMP_VELOCITY  -1.f * (WORLD_GRAVITY_Y * PLAYER_JUMP_TIME) / 2.0f
#define PLAYER_MOVE_DISTANCE 1.f
// ������
#define PLAYER_MOVE_VELOCITY (PLAYER_MOVE_DISTANCE / PLAYER_JUMP_TIME)

// �� ���� ����
#define ENEMY_MOVE_VELOCITY 5.f
#define ENEMY_RADIUS 0.5f
#define ENEMY_DESTROY_TIME 2.f
#define ENEMY_STOP_TIME_AFTER_ATTACKED 1.f

// ������Ʈ ���� ����
#define OBJECT_COIN_SIZE 0.8f

// UI ���� ����
#define UI_VIEW_SIZE 100.f
#define UI_CHARACTER_SCALE 0.1f

// ��ų ���� ����
#define SKILL_MELEE_ATTACK_COOLDOWN 0.f
#define SKILL_MELEE_ATTACK_DAMAGE 1.f
#define SKILL_MELEE_ATTACK_SKILLTIME 0.5f