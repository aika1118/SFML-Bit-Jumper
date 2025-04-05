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
	static void Init();
	static void Update(float deltaTime);
	static void DebugDraw(Renderer& renderer);

	static b2World* world; // 물리적 객체들이 존재하는 공간
	static b2Draw* debugDraw; // 디버그 드로잉 용

	static set<b2Body*> bodiesToDestroy; // 삭제 대기 리스트 (중복자료가 있으면 안되니 set 사용)
};