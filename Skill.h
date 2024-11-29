#pragma once
#include "Animation.h"
#include "Renderer.h"
#include "Resources.h"
#include "Config.h"
#include <iostream>

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

	bool isSkillEnd(); // 스킬 사용이 끝났는지 체크

protected:
	float _cooldown;
	float _damage;
	float _timer;
	float _skillTime;
	Animation _animation;
};