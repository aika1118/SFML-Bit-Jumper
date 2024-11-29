#include "SkillManager.h"

SkillManager::SkillManager() // 싱글톤 객체 (정적 인스턴스) 생성 시 기본 생성자 필요
	: _currentSkill(nullptr)
{
}

SkillManager& SkillManager::getInstance()
{
	static SkillManager instance; // 인스턴스는 최초 호출 시에만 생성됨
	return instance;
}

void SkillManager::AddSkill(int skillId, Skill* skill)
{
	_skills[skillId] = skill;
}

void SkillManager::Begin(int skillId)
{
	auto it = _skills.find(skillId);
	if (it == _skills.end())
	{
		cerr << "No exist skillId !" << endl;
		return;
	}

	_currentSkill = it->second;
	_currentSkill->Begin();
}

void SkillManager::Update(float deltaTime, int skillId)
{
	if (!isValidSkill(skillId))
		return;

	_currentSkill->Update(deltaTime);
}

void SkillManager::Render(Renderer& renderer, int skillId)
{
	if (!isValidSkill(skillId))
		return;

	_currentSkill->Render(renderer);
}

bool SkillManager::isSkillEnd(int skillId)
{
	if (!isValidSkill(skillId)) // return false 처리가 아닌, 별도 에러 처리로 수정 필요 
		return false;

	return _currentSkill->isSkillEnd();
}

bool SkillManager::isValidSkill(int skillId)
{
	return _currentSkill && _skills[skillId] == _currentSkill;
}
