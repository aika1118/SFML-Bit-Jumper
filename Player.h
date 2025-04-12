// File: Player.h
// Purpose: 플레이어 관리

#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Renderer.h"
#include "Physics.h"
#include "Resources.h"
#include "ContactListener.h"
#include "FixtureData.h"
#include "Animation.h"
#include "Coin.h"
#include "Enemy.h"
#include "SkillManager.h"

using namespace sf;
using namespace std;

enum class PlayerStatus { // PlayerStatus 접근자로만 접근 가능
	Idle,
	Moving,
	Jumping,
	Attacking
};

class Player : public ContactListener
{
public:
	void Begin();
	void Update(float deltaTime);
	void Draw(Renderer& renderer);

	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;

	Vector2f _position;
	float _angle;

	int _hp;
	bool _isDead = false;

	bool _facingLeft = false; // 캐릭터가 왼쪽을 바라보고 있는지 체크 (x축 속도의 부호로 판별)

	float getJudgementPercentage();
	string getJudgementCurrent();
	int getCurrentClearStage();
	int getCoin();

	bool bIsJumpImproved = false;

private:
	void HandleMove(float deltaTime, b2Vec2& velocity);
	void HandleJump(b2Vec2& velocity);
	void HandleSkill(float deltaTime, b2Vec2& velocity);

	Animation _runAnimation;
	Texture _textureToDraw;

	b2Body* body;
	int _groundContactCount = 0; // 여러 mapTile과 겹친 상태에서 일부 타일만 접촉 해제됐을 때도 땅에 붙어있음을 판별하기 위해 int로 관리
	int _jumpCount = 0;
	bool _isJumping = false;
	bool _previousSpaceState = false; // 프레임 별로 keyDown, keyReleased 판별을 위한 상태변수

	int _coins = 0;

	FixtureData _fixtureData; // FixtureDef에 userData.pointer로 연결해서 body에 부착
	FixtureData _sensorFixtureData; // FixtureDef에 userData.pointer로 연결해서 body에 부착
	FixtureData _headSensorFixtureData;

	bool _isFirstGrounded = false; // 땅에 한번 닿을 때마다 값을 기억하기 위한 boolean
	float _timeWhenJumpKeydown = 0.f; // 점프 시점 기록
	float _timeWhenGrounded = 0.f; // 땅에 닿은 시점 기록
	float _timeInOneCycle = 0.f; // 한 점프 사이클마다 점프시점, 땅에 닿은 시점을 기록하기 위한 time 값
	
	float _judgementPerfectCnt = 0;
	float _judgementGreatCnt = 0;
	float _judgementMissCnt = 0;
	float _judgementTotalCnt = 0; // 총 점프 횟수
	float _judgementPercentage = 0; // 총 점프 횟수와 판정 개수를 적절히 혼합하여 산출된 판정비율
	string _judgementCurrent = "-"; // 플레이어의 현재 판정

	PlayerStatus _playerStatus;
	int _currentSkillId;

	int _currentClearStage = -1; // 현재 클리어한 스테이지
	unordered_map<int, float> _stageScores; // 스테이지 클리어 점수 기록

	float _clearTime = 0.f;
};