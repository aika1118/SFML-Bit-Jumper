#pragma once
#include "Enemy.h"
#include <queue>

class EnemyPool
{
public:
	EnemyPool();
	~EnemyPool();
	Enemy* GetAvailableEnemy(Vector2f);
	void ScheduleEnemy(Enemy* enemy, float respawnTime);

	void Update(float deltaTime);
	void Render(Renderer& renderer);

	void Reset();

private:
	vector<Enemy*> enemies;
	queue<pair<Enemy*, float>> respawnQueue; // 리스폰 예정인 Enemy와 남은 리스폰 time 저장
};