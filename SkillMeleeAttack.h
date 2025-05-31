// 현재 미사용 class

#pragma once

#include "Skill.h"

class SkillMeleeAttack : public Skill
{
public:
	SkillMeleeAttack(int skillId);

	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& renderer) override;
};