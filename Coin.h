// File: Coin.h
// Purpose: ���� ������Ʈ ����

#pragma once
#include "Object.h"
#include "Animation.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Coin : public Object
{
public:
	/**
	 * ���� ��ü�� �ʱ�ȭ�մϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Begin() override;
	/**
	 * ���� ��ü�� ������Ʈ�մϴ�.
	 *
	 * @param deltaTime ������Ʈ�� ���Ǵ� �ð� �����Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Update(float deltaTime) override;
	/**
	 * ���� ��ü�� �������մϴ�.
	 *
	 * @param renderer �������� ���Ǵ� �������Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Render(Renderer& renderer) override;

private:
	Animation _animation;
};