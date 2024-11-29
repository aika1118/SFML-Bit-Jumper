#include "SkillManager.h"

SkillManager::SkillManager() // �̱��� ��ü (���� �ν��Ͻ�) ���� �� �⺻ ������ �ʿ�
	: _currentSkill(nullptr)
{
}

SkillManager& SkillManager::getInstance()
{
	static SkillManager instance; // �ν��Ͻ��� ���� ȣ�� �ÿ��� ������
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
	if (!isValidSkill(skillId)) // return false ó���� �ƴ�, ���� ���� ó���� ���� �ʿ� 
		return false;

	return _currentSkill->isSkillEnd();
}

bool SkillManager::isValidSkill(int skillId)
{
	return _currentSkill && _skills[skillId] == _currentSkill;
}
