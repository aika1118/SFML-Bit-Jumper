#pragma once
#include "Enemy.h"
#include <queue>

class EnemyPool
{
public:
	/**
	 * EnemyPool 클래스의 생성자입니다.
	 *
	 * 이 생성자는 EnemyPool 객체를 초기화합니다.
	 *
	 * @return 없음
	 */
	EnemyPool();
	/**
	 * EnemyPool 클래스의 소멸자입니다.
	 *
	 * 이 소멸자는 EnemyPool 객체를 삭제합니다.
	 *
	 * @return 없음
	 */
	~EnemyPool();
	/**
	 * 사용 가능한 적 객체를 반환하는 함수입니다.
	 *
	 * 이 함수는 사용 가능한 적 객체를 반환합니다.
	 *
	 * @param position 적 객체의 위치입니다.
	 *
	 * @return 사용 가능한 적 객체
	 */
	Enemy* GetAvailableEnemy(Vector2f);
	/**
	 * 적 객체를 재사용하기 위해 예약하는 함수입니다.
	 *
	 * 이 함수는 적 객체를 재사용하기 위해 예약합니다.
	 *
	 * @param enemy 예약할 적 객체입니다.
	 * @param respawnTime 재사용까지 남은 시간입니다.
	 *
	 * @return 없음
	 */
	void ScheduleEnemy(Enemy* enemy, float respawnTime);
	/**
	 * EnemyPool의 상태를 업데이트하는 함수입니다.
	 *
	 * 이 함수는 EnemyPool의 상태를 업데이트합니다.
	 *
	 * @param deltaTime 이전 프레임과 현재 프레임 사이의 시간 간격입니다.
	 *
	 * @return 없음
	 */
	void Update(float deltaTime);
	/**
	 * EnemyPool을 렌더링하는 함수입니다.
	 *
	 * 이 함수는 EnemyPool을 렌더링합니다.
	 *
	 * @param renderer 렌더링에 사용되는 렌더러 객체입니다.
	 *
	 * @return 없음
	 */
	void Render(Renderer& renderer);
	/**
	 * EnemyPool의 상태를 초기화하는 함수입니다.
	 *
	 * 이 함수는 EnemyPool의 상태를 초기화합니다.
	 *
	 * @return 없음
	 */
	void Reset();

private:
	vector<Enemy*> enemies; // pool에 등록된 적 관리
	queue<pair<Enemy*, float>> respawnQueue; // 리스폰 예정인 Enemy와 남은 리스폰 time 저장
};