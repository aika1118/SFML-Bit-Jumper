#pragma once
#include <string>
using namespace std;

// ��� ����
const float M_PI = 22.f / 7.f;

// �� ���� ����
const float MAP_CELL_SIZE = 1.f;
const string MAP_STAGE_1 = "./resources/images/stage1.png";
const bool MAP_IS_DEBUG_DRAW = false;

// ī�޶� ���� ����
const float CAMERA_ZOOM_LEVEL = 20.f;

// �÷��̾� ���� ����
const float PLAYER_SIZE_RATIO = 1.28f;
const float PLAYER_SIZE_WIDTH = 0.5f;
const float PLAYER_SIZE_HEIGHT = (PLAYER_SIZE_WIDTH * PLAYER_SIZE_RATIO);
const float PLAYER_SENSOR_HX = (PLAYER_SIZE_WIDTH / 2.f) * 0.8f;
const float PLAYER_SENSOR_HY = 0.01f;
const int PLAYER_MAX_JUMP_COUNT = 1;
const float PLAYER_INVINCIBILITY_TIME = 1.f;

// ���� ����

// DJMAX ���� (����)
//const float JUDGEMENT_PERFECT = 41.67f;
//const float JUDGEMENT_GREAT = 60.f;

// EZ2ON ���� (�����)
const float JUDGEMENT_PERFECT = 22.f;
const float JUDGEMENT_GREAT = 60.f;


// ���� ���� ����
const float WORLD_GRAVITY_X = 0.f;
const int WORLD_VELOCITY_ITERATION = 8;
const int WORLD_POSITION_ITERATION = 3;

// �÷��̾� ���� ���� ����
const float PLAYER_JUMP_HEIGHT = 2.f;
const float PLAYER_JUMP_TIME = 0.5f;
// ������
const float WORLD_GRAVITY_Y = (8.0f * PLAYER_JUMP_HEIGHT) / (PLAYER_JUMP_TIME * PLAYER_JUMP_TIME);
// ������
const float PLAYER_JUMP_VELOCITY =  -1.f * (WORLD_GRAVITY_Y * PLAYER_JUMP_TIME) / 2.0f;
const float PLAYER_MOVE_DISTANCE = 1.f;
// ������
const float PLAYER_MOVE_VELOCITY = (PLAYER_MOVE_DISTANCE / PLAYER_JUMP_TIME);

// �� ���� ����
const float ENEMY_MOVE_VELOCITY = 5.f;
const float ENEMY_RADIUS = 0.5f;
const float ENEMY_DESTROY_TIME = 2.f;
const float ENEMY_STOP_TIME_AFTER_ATTACKED = 1.f;

// ������Ʈ ���� ����
const float OBJECT_COIN_SIZE = 0.8f;

// UI ���� ����
const float UI_VIEW_SIZE = 100.f;
const float UI_CHARACTER_SCALE = 0.1f;

// ��ų ���� ����
const float SKILL_MELEE_ATTACK_COOLDOWN = 0.f;
const float SKILL_MELEE_ATTACK_DAMAGE = 1.f;
const float SKILL_MELEE_ATTACK_SKILLTIME = 0.5f;