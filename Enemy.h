// File: Enemy.h
// Purpose: �� ��ü�� ����

#pragma once

#include "Object.h"
#include "Animation.h"
#include "FixtureData.h"
#include "Config.h"
#include "Resources.h"
#include "Physics.h"
#include "box2d/box2d.h"


class Enemy : public Object
{
public:
	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& renderer) override;

	bool IsDead();
	void SetActive(bool);
	bool IsActive() const;
	void Reset(Vector2f position);
	Vector2f GetSpawnPosition() const;
	void SetDead();

	int _hp;
	bool _isEnemyAttacked; // �ǰ� �������� üũ

private:
	Animation _animation;
	float _movement;
	bool _isDead;
	float _destroyTimer; // ���� �� �󸶳� texture�� �����ų�� �����ϴ� ����
	bool _facingLeft;
	FixtureData _fixtureData;
	bool _isActive;
	Vector2f _spawnPosition;

	float _hitTimer; // �ǰ� �� ���Ͱ� �����ִ� �ð� ����
	
};