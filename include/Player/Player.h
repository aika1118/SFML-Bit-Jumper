// File: Player.h
// Purpose: 플레이어 관리

#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Renderer.h"
#include "Physics.h"
#include "Resources.h"
#include "ContactListener.h"
#include "FixtureData.h"
#include "Animation.h"
#include "Coin.h"
#include "Enemy.h"
#include "SkillManager.h"

using namespace sf;
using namespace std;

enum class PlayerStatus { // PlayerStatus 접근자로만 접근 가능
	Idle,
	Moving,
	Jumping,
	Attacking
};

class Player : public ContactListener
{
public:
	/**
	 * @brief 플레이어 초기화를 수행합니다.
	 * 플레이어 초기화는 플레이어의 상태를 초기값으로 설정합니다.
	 */
	void Begin();
	/**
	 * @brief 플레이어를 업데이트합니다.
	 * 업데이트는 플레이어의 상태를 변경합니다.
	 *
	 * @param deltaTime 이전 프레임과 현재 프레임 사이의 시간 차이입니다.
	 */
	void Update(float deltaTime);
	/**
	 * @brief 플레이어를 렌더링합니다.
	 * 렌더링은 플레이어의 그래픽을 화면에 출력합니다.
	 *
	 * @param renderer 렌더링을 수행할 렌더러입니다.
	 */
	void Draw(Renderer& renderer);
	/**
	 * @brief 플레이어가 다른 물체와 접촉할 때 호출됩니다.
	 * 이 함수는 플레이어와 다른 물체의 접촉을 처리합니다.
	 *
	 * @param self 플레이어 자신입니다.
	 * @param other 다른 물체입니다.
	 */
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	/**
	 * @brief 플레이어가 다른 물체와의 접촉이 끝났을 때 호출됩니다.
	 * 이 함수는 플레이어와 다른 물체의 접촉이 끝났을 때 처리합니다.
	 *
	 * @param self 플레이어 자신입니다.
	 * @param other 다른 물체입니다.
	 */
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;
	/**
	 * @brief 플레이어의 판정 비율을 가져옵니다.
	 * 판정 비율은 플레이어의 정확도를 평가하는 데 사용됩니다.
	 *
	 * @return 플레이어의 판정 비율입니다.
	 */
	float getJudgementPercentage();
	/**
	 * @brief 플레이어의 현재 판정을 가져옵니다.
	 *
	 * @return 플레이어의 현재 판정입니다.
	 */
	string getJudgementCurrent();
	/**
	 * @brief 플레이어가 클리어한 스테이지를 가져옵니다.
	 *
	 * @return 플레이어가 클리어한 스테이지입니다.
	 */
	int getCurrentClearStage();
	/**
	 * @brief 플레이어가 가지고 있는 코인을 가져옵니다.
	 *
	 * @return 플레이어가 가지고 있는 코인입니다.
	 */
	int getCoin();

	Vector2f _position; // 플레이어의 위치
	float _angle; // 플레이어의 각도
	int _hp; // 플레이어의 HP
	bool _isDead = false; // 플레이어 생존 상태
	bool _facingLeft = false; // 캐릭터가 왼쪽을 바라보고 있는지 체크 (x축 속도의 부호로 판별)
	bool bIsJumpImproved = false; // 버섯을 먹어 플레이어가 높이 점프할 수 있는 상태인지

private:
	/**
	 * @brief 플레이어의 이동을 처리합니다.
	 *
	 * @param deltaTime 이전 프레임과 현재 프레임 사이의 시간 차이입니다.
	 * @param velocity 플레이어의 속도입니다.
	 */
	void HandleMove(float deltaTime, b2Vec2& velocity);
	/**
	 * @brief 플레이어의 점프를 처리합니다.
	 *
	 * @param velocity 플레이어의 속도입니다.
	 */
	void HandleJump(b2Vec2& velocity);
	/**
	 * @brief 플레이어의 스킬을 처리합니다.
	 *
	 * @param deltaTime 이전 프레임과 현재 프레임 사이의 시간 차이입니다.
	 * @param velocity 플레이어의 속도입니다.
	 */
	void HandleSkill(float deltaTime, b2Vec2& velocity);

	Animation _runAnimation; // 플레이어 달리는 animation 저장
	Texture _textureToDraw; // 현재 그릴 texture

	b2Body* body;
	int _groundContactCount = 0; // 여러 mapTile과 겹친 상태에서 일부 타일만 접촉 해제됐을 때도 땅에 붙어있음을 판별하기 위해 int로 관리
	int _jumpCount = 0; // 연속점프횟수
	bool _isJumping = false; // 플레이어가 점프중인가
	bool _previousSpaceState = false; // 프레임 별로 keyDown, keyReleased 판별을 위한 상태변수

	int _coins = 0; // 소지중인 coin 개수

	bool _isFirstGrounded = false; // 땅에 한번 닿을 때마다 값을 기억하기 위한 boolean
	float _timeWhenJumpKeydown = 0.f; // 점프 시점 기록
	float _timeWhenGrounded = 0.f; // 땅에 닿은 시점 기록
	float _timeInOneCycle = 0.f; // 한 점프 사이클마다 점프시점, 땅에 닿은 시점을 기록하기 위한 time 값
	
	float _judgementPerfectCnt = 0; // perfect 판정 수
	float _judgementGreatCnt = 0; // great 판정 수
	float _judgementMissCnt = 0; // miss 판정 수
	float _judgementTotalCnt = 0; // 총 점프 횟수
	float _judgementPercentage = 0; // 총 점프 횟수와 판정 개수를 적절히 혼합하여 산출된 판정비율
	string _judgementCurrent = "-"; // 플레이어의 현재 판정

	PlayerStatus _playerStatus; // player가 현재 취하고 있는 상태값
	int _currentSkillId; // skill id 저장

	int _currentClearStage = -1; // 현재 클리어한 스테이지
	unordered_map<int, float> _stageScores; // 스테이지 클리어 점수 기록

	float _clearTime = 0.f; // 스테이지 클리어 시간
};