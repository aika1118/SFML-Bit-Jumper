// File: Enemy.h
// Purpose: 적 객체를 관리

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
	 * 적 객체의 초기화를 담당하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 초기 상태를 설정합니다.
	 *
	 * @return 없음
	 */
	virtual void Begin() override;
	/**
	 * 적 객체의 상태를 업데이트 하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 위치, 애니메이션, 물리적인 상태 등을 업데이트합니다.
	 *
	 * @param deltaTime 이전 프레임과 현재 프레임 사이의 시간 간격입니다.
	 *
	 * @return 없음
	 */
	virtual void Update(float deltaTime) override;
	/**
	 * 적 객체를 렌더링하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 그래픽을 화면에 출력합니다.
	 *
	 * @param renderer 렌더링에 사용되는 렌더러 객체입니다.
	 *
	 * @return 없음
	 */
	virtual void Render(Renderer& renderer) override;

	/**
	 * 적 객체의 사망 여부를 반환하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 사망 상태를 확인합니다.
	 *
	 * @return 적 객체가 사망한 경우 true, 그렇지 않은 경우 false
	 */
	bool IsDead();
	/**
	 * 적 객체의 활성화 여부를 설정하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 활성화 상태를 설정합니다.
	 *
	 * @param active 활성화 여부 (true: 활성화, false: 비활성화)
	 *
	 * @return 없음
	 */
	void SetActive(bool);
	/**
	 * 적 객체의 활성화 여부를 반환하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 현재 활성화 상태를 확인합니다.
	 *
	 * @return 적 객체가 활성화된 경우 true, 그렇지 않은 경우 false
	 */
	bool IsActive() const;
	/**
	 * 적 객체의 상태를 초기화하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 위치, 애니메이션, 물리적인 상태 등을 초기화합니다.
	 *
	 * @param position 초기화할 위치입니다.
	 *
	 * @return 없음
	 */
	void Reset(Vector2f position);
	/**
	 * 적 객체의 스폰 위치를 반환하는 함수입니다.
	 *
	 * 이 함수는 적 객체가 생성된 위치를 반환합니다.
	 *
	 * @return 적 객체의 스폰 위치
	 */
	Vector2f GetSpawnPosition() const;
	/**
	 * 적 객체의 사망 상태를 설정하는 함수입니다.
	 *
	 * 이 함수는 적 객체의 사망 상태를 설정합니다.
	 *
	 * @return 없음
	 */
	void SetDead();

	int _hp; // 적 HP
	bool _isEnemyAttacked; // 피격 상태인지 체크

private:
	Animation _animation; // 적 animation 관리
	float _movement; // 적 속도
	bool _isDead; // 적 사망상태 체크
	float _destroyTimer; // 죽은 후 얼마나 texture를 노출시킬지 관리하는 변수
	bool _facingLeft; // 적이 왼쪽를 바라보고 있는가
	bool _isActive; // 적이 활성화 상태인가
	Vector2f _spawnPosition; // 적 스폰 위치

	float _hitTimer; // 피격 후 몬스터가 멈춰있는 시간 관리
	
};