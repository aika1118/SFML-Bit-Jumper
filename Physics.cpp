#include "Physics.h"

b2World* Physics::world; // static 정의
b2Draw* Physics::debugDraw; // static 정의
vector<b2Body*> Physics::bodiesToDestroy; // static 정의

void Physics::Init()
{
	if (world)
		delete world; // 기존 world 초기화

	world = new b2World(b2Vec2(WORLD_GRAVITY_X, WORLD_GRAVITY_Y));
	world->SetDebugDraw(debugDraw);
	world->SetContactListener(new MyGlobalContactListener()); // 충돌 감지를 위한 ContactListner를 world에 설정

}

void Physics::Update(float deltaTime)
{
	// 물리 세계에서 한 프레임만큼 물리 시뮬레이션을 진행
	world->Step(deltaTime, WORLD_VELOCITY_ITERATION, WORLD_POSITION_ITERATION); // deltaTime 경과에 따른 속도와 위치 업데이트 (속도 및 위치 계산을 위한 연산 횟수)
	
	// Step 이후 삭제 대기 리스트 처리 (step중 destroy 할 수 없기 때문에 이렇게 처리)
	for (b2Body* body : bodiesToDestroy)
		Physics::world->DestroyBody(body);

	bodiesToDestroy.clear();
}

void Physics::DebugDraw(Renderer& renderer)
{
	if (!debugDraw)
	{
		debugDraw = new MyDebugDraw(renderer._target);
		debugDraw->SetFlags(b2Draw::e_shapeBit);
		//debugDraw->SetFlags(b2Draw::e_aabbBit); // 객체를 포함하는 직사각형 경계 상자
		world->SetDebugDraw(debugDraw);
	}

	world->DebugDraw();
}
