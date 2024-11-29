#include "SkillMeleeAttack.h"
#include "Game.h"


SkillMeleeAttack::SkillMeleeAttack(int skillId)
	:Skill(skillId)
{
}

void SkillMeleeAttack::Begin()
{
	_timer = 0.f;
}

void SkillMeleeAttack::Update(float deltaTime)
{
	_timer += deltaTime;
	_animation.Update(deltaTime);
}

void SkillMeleeAttack::Render(Renderer& renderer)
{
	renderer.Draw(_animation.GetTexture(), Game::getInstance().getPlayerPosition(), Vector2f(1.f, PLAYER_NORMALIZED_HEIGHT), Game::getInstance().getPlayerAngle());
}
