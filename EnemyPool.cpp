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

	// Ǯ ���� �� ���� Ȯ��
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
	while (!respawnQueue.empty()) // ��� ������ ��� Enemy�� ���� update ó��
	{
		pair<Enemy*, float> pair = respawnQueue.front();
		pair.second -= deltaTime;
		if (pair.second <= 0.f) // �������� ���� ã�ƿ�
		{
			pair.first->Reset(pair.first->GetSpawnPosition());
			pair.first->SetActive(true);
		}
		else tempQueue.push(pair);

		respawnQueue.pop();
	}

	respawnQueue = tempQueue; // ��� ���� ������ ������ ������Ʈ�� ���� ������

	for (Enemy* enemy : enemies) // Ȱ��ȭ�� ��� �� update ó��
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
		enemy->Begin(); // ���� world�� �µ��� _body �����
		enemy->SetActive(false);
	}

	respawnQueue = queue<pair<Enemy*, float>>();
}
