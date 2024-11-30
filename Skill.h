#pragma once
#include "Animation.h"
#include "Renderer.h"
#include "Resources.h"
#include "Config.h"
#include "box2d/box2d.h"
#include "Physics.h"
#include <iostream>

// 스킬 리스트 enum 정의
enum class SkillList {
	MELEE_START = 100000000,
	MELEE_ATTACK,
	MELEE_ATTACK2,
	MELEE_END
};

class Skill
{
public:
	Skill(int skillId);

	virtual void Begin() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;

	bool isSkillEnd(); // 스킬 사용이 끝났는지 체크 (timer가 skillTime을 넘은 경우 끝난 것)

protected:
	float _cooldown; // 스킬 쿨타임
	float _damage; // 스킬 데미지
	float _timer; // 스킬 유지 시간을 제한하기 위한 타이머
	float _skillTime; // 스킬 유지 시간
	Animation _animation; // 스킬 사용 animation

	b2Body* _attackBody; // b2body for attack box
	FixtureData _attackFixtureData; // FixtureData for attack box
};