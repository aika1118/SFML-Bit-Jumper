#include "EnemyPool.h"

EnemyPool::EnemyPool()
{
	for (int i = 0; i < ENEMY_POOL_SIZE; ++i)
	{
		Enemy* enemy = new Enemy();
		enemy->Begin();
		enemy->SetActive(false);
		enemies.push_back(enemy);
	}
		
}

EnemyPool::~EnemyPool()
{
	for (Enemy* enemy : enemies)
		delete enemy;
}

Enemy* EnemyPool::GetAvailableEnemy(Vector2f position)
{
	for (Enemy* enemy : enemies)
	{
		if (!enemy->IsActive())
		{
			enemy->Reset(position);
			enemy->SetActive(true);
			return enemy;
		}
	}

	// 풀 부족 시 동적 확장
	Enemy* newEnemy = new Enemy();
	newEnemy->Begin();
	newEnemy->Reset(position);
	newEnemy->SetActive(true);
	enemies.push_back(newEnemy);
	cout << "Enemy Pool Expand !" << endl;
	return newEnemy;
}

void EnemyPool::ScheduleEnemy(Enemy* enemy, float respawnTime)
{
	respawnQueue.push({enemy, respawnTime});
}

void EnemyPool::Update(float deltaTime)
{
	queue<pair<Enemy*, float>> tempQueue;
	while (!respawnQueue.empty()) // 모든 리스폰 대상 Enemy에 대해 update 처리
	{
		pair<Enemy*, float> pair = respawnQueue.front();
		pair.second -= deltaTime;
		if (pair.second <= 0.f) // 리스폰할 때가 찾아옴
		{
			pair.first->Reset(pair.first->GetSpawnPosition());
			pair.first->SetActive(true);
		}
		else tempQueue.push(pair);

		respawnQueue.pop();
	}

	respawnQueue = tempQueue; // 모든 적의 리스폰 정보가 업데이트된 정보 가져옴

	for (Enemy* enemy : enemies) // 활성화된 모든 적 update 처리
	{
		if (enemy->IsActive()) enemy->Update(deltaTime);
	}
}

void EnemyPool::Render(Renderer& renderer)
{
	for (Enemy* enemy : enemies)
	{
		if (enemy->IsActive()) enemy->Render(renderer);
	}
}

void EnemyPool::Reset()
{
	for (Enemy* enemy : enemies)
	{
		enemy->Begin(); // 현재 world에 맞도록 _body 재생성
		enemy->SetActive(false);
	}

	respawnQueue = queue<pair<Enemy*, float>>();
}
