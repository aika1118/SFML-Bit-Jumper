// File: Enemy.h
// Purpose: �� ��ü�� ����

#pragma once

#include "Object.h"
#include "Animation.h"
#include "FixtureData.h"
#include "Config.h"
#include "Resources.h"
#include "Physics.h"
#include "box2d/box2d.h"


class Enemy : public Object
{
public:
	/**
	 * �� ��ü�� �ʱ�ȭ�� ����ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� �ʱ� ���¸� �����մϴ�.
	 *
	 * @return ����
	 */
	virtual void Begin() override;
	/**
	 * �� ��ü�� ���¸� ������Ʈ �ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� ��ġ, �ִϸ��̼�, �������� ���� ���� ������Ʈ�մϴ�.
	 *
	 * @param deltaTime ���� �����Ӱ� ���� ������ ������ �ð� �����Դϴ�.
	 *
	 * @return ����
	 */
	virtual void Update(float deltaTime) override;
	/**
	 * �� ��ü�� �������ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� �׷����� ȭ�鿡 ����մϴ�.
	 *
	 * @param renderer �������� ���Ǵ� ������ ��ü�Դϴ�.
	 *
	 * @return ����
	 */
	virtual void Render(Renderer& renderer) override;

	/**
	 * �� ��ü�� ��� ���θ� ��ȯ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� ��� ���¸� Ȯ���մϴ�.
	 *
	 * @return �� ��ü�� ����� ��� true, �׷��� ���� ��� false
	 */
	bool IsDead();
	/**
	 * �� ��ü�� Ȱ��ȭ ���θ� �����ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� Ȱ��ȭ ���¸� �����մϴ�.
	 *
	 * @param active Ȱ��ȭ ���� (true: Ȱ��ȭ, false: ��Ȱ��ȭ)
	 *
	 * @return ����
	 */
	void SetActive(bool);
	/**
	 * �� ��ü�� Ȱ��ȭ ���θ� ��ȯ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� ���� Ȱ��ȭ ���¸� Ȯ���մϴ�.
	 *
	 * @return �� ��ü�� Ȱ��ȭ�� ��� true, �׷��� ���� ��� false
	 */
	bool IsActive() const;
	/**
	 * �� ��ü�� ���¸� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� ��ġ, �ִϸ��̼�, �������� ���� ���� �ʱ�ȭ�մϴ�.
	 *
	 * @param position �ʱ�ȭ�� ��ġ�Դϴ�.
	 *
	 * @return ����
	 */
	void Reset(Vector2f position);
	/**
	 * �� ��ü�� ���� ��ġ�� ��ȯ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� ������ ��ġ�� ��ȯ�մϴ�.
	 *
	 * @return �� ��ü�� ���� ��ġ
	 */
	Vector2f GetSpawnPosition() const;
	/**
	 * �� ��ü�� ��� ���¸� �����ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� ��� ���¸� �����մϴ�.
	 *
	 * @return ����
	 */
	void SetDead();

	int _hp; // �� HP
	bool _isEnemyAttacked; // �ǰ� �������� üũ

private:
	Animation _animation; // �� animation ����
	float _movement; // �� �ӵ�
	bool _isDead; // �� ������� üũ
	float _destroyTimer; // ���� �� �󸶳� texture�� �����ų�� �����ϴ� ����
	bool _facingLeft; // ���� ���ʸ� �ٶ󺸰� �ִ°�
	bool _isActive; // ���� Ȱ��ȭ �����ΰ�
	Vector2f _spawnPosition; // �� ���� ��ġ

	float _hitTimer; // �ǰ� �� ���Ͱ� �����ִ� �ð� ����
	
};