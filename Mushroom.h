#pragma once

#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Mushroom : public Object
{
public:
	/**
	 * @brief mushroom �ʱ�ȭ�� ȣ��
	 */
	void Begin() override;
	/**
	 * @brief mushroom ���� ������Ʈ
	 * @param deltaTime �� ������ ���� �ð�
	 */
	void Update(float deltaTime) override;
	/**
	 * @brief mushroom ������
	 * @param renderer �������� ���� object
	 */
	void Render(Renderer& renderer) override;

private:
	float _jumpImproveTimeLength = 5.f; // jump improve time when player eats a mushroom 
	float _timer = 0.f;
};