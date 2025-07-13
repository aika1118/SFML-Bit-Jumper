#pragma once
#include "Enemy.h"
#include <queue>

class EnemyPool
{
public:
	/**
	 * EnemyPool Ŭ������ �������Դϴ�.
	 *
	 * �� �����ڴ� EnemyPool ��ü�� �ʱ�ȭ�մϴ�.
	 *
	 * @return ����
	 */
	EnemyPool();
	/**
	 * EnemyPool Ŭ������ �Ҹ����Դϴ�.
	 *
	 * �� �Ҹ��ڴ� EnemyPool ��ü�� �����մϴ�.
	 *
	 * @return ����
	 */
	~EnemyPool();
	/**
	 * ��� ������ �� ��ü�� ��ȯ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� ��� ������ �� ��ü�� ��ȯ�մϴ�.
	 *
	 * @param position �� ��ü�� ��ġ�Դϴ�.
	 *
	 * @return ��� ������ �� ��ü
	 */
	Enemy* GetAvailableEnemy(Vector2f);
	/**
	 * �� ��ü�� �����ϱ� ���� �����ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� �� ��ü�� �����ϱ� ���� �����մϴ�.
	 *
	 * @param enemy ������ �� ��ü�Դϴ�.
	 * @param respawnTime ������� ���� �ð��Դϴ�.
	 *
	 * @return ����
	 */
	void ScheduleEnemy(Enemy* enemy, float respawnTime);
	/**
	 * EnemyPool�� ���¸� ������Ʈ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� EnemyPool�� ���¸� ������Ʈ�մϴ�.
	 *
	 * @param deltaTime ���� �����Ӱ� ���� ������ ������ �ð� �����Դϴ�.
	 *
	 * @return ����
	 */
	void Update(float deltaTime);
	/**
	 * EnemyPool�� �������ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� EnemyPool�� �������մϴ�.
	 *
	 * @param renderer �������� ���Ǵ� ������ ��ü�Դϴ�.
	 *
	 * @return ����
	 */
	void Render(Renderer& renderer);
	/**
	 * EnemyPool�� ���¸� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
	 *
	 * �� �Լ��� EnemyPool�� ���¸� �ʱ�ȭ�մϴ�.
	 *
	 * @return ����
	 */
	void Reset();

private:
	vector<Enemy*> enemies; // pool�� ��ϵ� �� ����
	queue<pair<Enemy*, float>> respawnQueue; // ������ ������ Enemy�� ���� ������ time ����
};