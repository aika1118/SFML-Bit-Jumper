#pragma once

// Purpose: ���� ������Ʈ ����

#pragma once
#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Spike : public Object
{
public:
	/**
	* ������Ʈ �ʱ�ȭ �Լ��Դϴ�.
	* ������Ʈ�� �ʱ� ���¸� �����մϴ�.
	*
	*/
	void Begin() override;
	/**
	* ������Ʈ ������Ʈ �Լ��Դϴ�.
	* ������Ʈ�� ���¸� ������Ʈ �մϴ�.
	* @param deltaTime �ð� ����
	*/
	void Update(float deltaTime) override;
	/**
	* ������Ʈ ������ �Լ��Դϴ�.
	* ������Ʈ�� ȭ�鿡 �׸��� ���� ȣ��˴ϴ�.
	* @param renderer ������ ��ü
	*/
	void Render(Renderer& renderer) override;
};