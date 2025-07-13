#pragma once

#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class BoxFragile : public Object
{
public:
	/**
	 * ��ü�� �ʱ�ȭ�մϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Begin() override;
	/**
	 * ��ü�� ������Ʈ �մϴ�.
	 *
	 * @param deltaTime ������Ʈ�� ���Ǵ� �ð� �����Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Update(float deltaTime) override;
	/**
	 * ��ü�� �������մϴ�.
	 *
	 * @param renderer �������� ���Ǵ� �������Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Render(Renderer& renderer) override;
	/**
	 * ��ü�� ���ظ� �����ϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void TakeDamage();
	/**
	 * ��ü�� ���� HP�� ��ȯ�մϴ�.
	 *
	 * @return ��ü�� ���� HP�Դϴ�.
	 *
	 * @throws ����
	 */
	int getHP();

private:
	/**
	 * ��ü�� ���� HP�� ����� �ؽ�ó�� ��ȯ�մϴ�.
	 *
	 * @return ��ü�� ���� HP�� ����� �ؽ�ó�Դϴ�.
	 *
	 * @throws ����
	 */
	Texture GetTextureBasedOnHP() const;
	int _hp = 2;
};