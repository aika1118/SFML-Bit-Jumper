// File: Enemy.h
// Purpose: 적 객체를 관리

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

	virtual void destroyBody() override;
	bool IsDead();

	int _hp;
	bool _isEnemyAttacked; // 피격 상태인지 체크

private:
	Animation _animation;
	float _movement;
	bool _isDead;
	float _destroyTimer; // 죽은 후 얼마나 texture를 노출시킬지 관리하는 변수
	bool _facingLeft;
	FixtureData _fixtureData;

	float _hitTimer; // 피격 후 몬스터가 멈춰있는 시간 관리
	
};