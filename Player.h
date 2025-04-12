// File: Player.h
// Purpose: �÷��̾� ����

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

enum class PlayerStatus { // PlayerStatus �����ڷθ� ���� ����
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

	bool _facingLeft = false; // ĳ���Ͱ� ������ �ٶ󺸰� �ִ��� üũ (x�� �ӵ��� ��ȣ�� �Ǻ�)

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
	int _groundContactCount = 0; // ���� mapTile�� ��ģ ���¿��� �Ϻ� Ÿ�ϸ� ���� �������� ���� ���� �پ������� �Ǻ��ϱ� ���� int�� ����
	int _jumpCount = 0;
	bool _isJumping = false;
	bool _previousSpaceState = false; // ������ ���� keyDown, keyReleased �Ǻ��� ���� ���º���

	int _coins = 0;

	FixtureData _fixtureData; // FixtureDef�� userData.pointer�� �����ؼ� body�� ����
	FixtureData _sensorFixtureData; // FixtureDef�� userData.pointer�� �����ؼ� body�� ����
	FixtureData _headSensorFixtureData;

	bool _isFirstGrounded = false; // ���� �ѹ� ���� ������ ���� ����ϱ� ���� boolean
	float _timeWhenJumpKeydown = 0.f; // ���� ���� ���
	float _timeWhenGrounded = 0.f; // ���� ���� ���� ���
	float _timeInOneCycle = 0.f; // �� ���� ����Ŭ���� ��������, ���� ���� ������ ����ϱ� ���� time ��
	
	float _judgementPerfectCnt = 0;
	float _judgementGreatCnt = 0;
	float _judgementMissCnt = 0;
	float _judgementTotalCnt = 0; // �� ���� Ƚ��
	float _judgementPercentage = 0; // �� ���� Ƚ���� ���� ������ ������ ȥ���Ͽ� ����� ��������
	string _judgementCurrent = "-"; // �÷��̾��� ���� ����

	PlayerStatus _playerStatus;
	int _currentSkillId;

	int _currentClearStage = -1; // ���� Ŭ������ ��������
	unordered_map<int, float> _stageScores; // �������� Ŭ���� ���� ���

	float _clearTime = 0.f;
};