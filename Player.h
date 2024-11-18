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

using namespace sf;
using namespace std;

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

private:

	b2Body* body;
	bool _facingLeft = false; // ĳ���Ͱ� ������ �ٶ󺸰� �ִ��� üũ (x�� �ӵ��� ��ȣ�� �Ǻ�)
	int _groundContactCount = 0; // ���� mapTile�� ��ģ ���¿��� �Ϻ� Ÿ�ϸ� ���� �������� ���� ���� �پ������� �Ǻ��ϱ� ���� int�� ����
	int _jumpCount = 0;
	bool _isJumping = false;
	bool _previousSpaceState = false; // ������ ���� keyDown, keyReleased �Ǻ��� ���� ���º���

	FixtureData _fixtureData; // FixtureDef�� userData.pointer�� �����ؼ� body�� ����
	FixtureData _sensorFixtureData; // FixtureDef�� userData.pointer�� �����ؼ� body�� ����
};