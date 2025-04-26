#pragma once
#include "Animation.h"
#include "Renderer.h"
#include "Resources.h"
#include "Config.h"
#include "box2d/box2d.h"
#include "Physics.h"
#include <iostream>

// ��ų ����Ʈ enum ����
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

	bool isSkillEnd(); // ��ų ����� �������� üũ (timer�� skillTime�� ���� ��� ���� ��)

	virtual ~Skill() = default; // �����Լ��� �־ ������ �ִ� Ŭ������. ���� ����Ҹ��� ���� �� default�� ���� (�Ļ�Ŭ������ �������� �Ҹ��� ����)

protected:
	float _cooldown; // ��ų ��Ÿ��
	float _damage; // ��ų ������
	float _timer; // ��ų ���� �ð��� �����ϱ� ���� Ÿ�̸�
	float _skillTime; // ��ų ���� �ð�
	Animation _animation; // ��ų ��� animation

	b2Body* _attackBody; // b2body for attack box
};