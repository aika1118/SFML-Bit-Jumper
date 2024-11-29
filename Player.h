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
	~Player();

	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;

	Vector2f _position;
	float _angle;

	int _hp;
	bool _isDead;

	bool _facingLeft = false; // ĳ���Ͱ� ������ �ٶ󺸰� �ִ��� üũ (x�� �ӵ��� ��ȣ�� �Ǻ�)

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

	PlayerStatus _playerStatus;
	int _currentSkillId;
};