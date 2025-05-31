// ���� �̻�� class

#pragma once

#include <unordered_map>
#include "Skill.h"
#include <iostream>


using namespace std;

class SkillManager 
{
public:
	static SkillManager& getInstance(); // �̱���

	void AddSkill(int skillId, Skill* skill); // ��ų �߰�
	void Begin(int skillId);
	void Update(float deltaTime, int skillId); 
	void Render(Renderer& renderer, int skillId);
	bool isSkillEnd(int skillId); // ��ų ����� �������� üũ
	bool isValidSkill(int skillId); // ��ų ��ȿ�� �˻�
	
private:
	SkillManager(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	SkillManager(const SkillManager&) = delete; // ��������� ����
	SkillManager& operator=(const SkillManager&) = delete; // ���Կ����� ����
	
	unordered_map<int, Skill*> _skills; // ��ų ID�� skill ��ü ����
	Skill* _currentSkill;
};