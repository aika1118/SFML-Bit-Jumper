// File: Physics.h
// Purpose: 게임에서 물리적인 모든 상호작용을 관리

#pragma once
#include <box2d/b2_world.h>
#include <vector>
#include "Renderer.h"
#include "Config.h"
#include "MyDebugDraw.h"
#include "MyGlobalContactListener.h"
#include "set"

using namespace std;

class b2Draw;

// 전체 게임에서 물리적인 모든 상호작용을 관리하기 때문에 static 선언
class Physics
{
public:
	/**
	* 물리 엔진 초기화 함수입니다.
	* 물리 엔진을 초기화합니다.
	*/
	static void Init();
	/**
	* 물리 엔진 업데이트 함수입니다.
	* 물리 엔진의 상태를 업데이트 합니다.
	* @param deltaTime 시간 차이
	*/
	static void Update(float deltaTime);
	/**
	* 물리 엔진 디버그 드로잉 함수입니다.
	* 물리 엔진의 디버그 정보를 화면에 그리기 위해 호출됩니다.
	* @param renderer 렌더러 객체
	*/
	static void DebugDraw(Renderer& renderer);
	/**
	* world를 소멸하기 위한 함수입니다.
	* 
	*/
	static void Shutdown();
	/**
	* fixtureData 포인터를 Physics에서 관리하기 위한 함수
	* @param fixtureData world에 생성된 fixtureData 객체
	*/
	static void AddFixtureData(unique_ptr<FixtureData> fixtureData); 

	static inline b2World* world; // 물리적 객체들이 존재하는 world (inline을 통한 헤더파일에서 static 변수 단일 정의 보장)
	static inline b2Draw* debugDraw; // 디버그 드로잉 용

	static inline set<b2Body*> bodiesToDestroy; // 삭제 대기 리스트 (중복자료가 있으면 안되니 set 사용)
	static inline set<pair<b2Body*, bool>> bodiesSetEnabled; // body에 대해 SetEnable 처리할 목록 (삭제하지 않고 비활성화 상태로 만듦, pool에서 활용)
	

private:
	static inline vector<unique_ptr<FixtureData>> fixtureDataList; // fixtureData 포인터 관리용 (차후 world 파괴 시 physics.h 측에서 destroy 처리)
};