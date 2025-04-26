#include "Physics.h"

b2World* Physics::world; // static 정의
b2Draw* Physics::debugDraw; // static 정의
set<b2Body*> Physics::bodiesToDestroy; // static 정의
set<pair<b2Body*, bool>> Physics::bodiesSetEnabled;
vector<unique_ptr<FixtureData>> Physics::fixtureDataList;

void Physics::Init()
{
	if (world)
		delete world; // 기존 world 초기화 (b2Body 관련 리소스도 안전하게 정리)

	bodiesToDestroy.clear();
	bodiesSetEnabled.clear();
	fixtureDataList.clear();

	world = new b2World(b2Vec2(WORLD_GRAVITY_X, WORLD_GRAVITY_Y));
	world->SetDebugDraw(debugDraw);
	world->SetContactListener(new MyGlobalContactListener()); // 충돌 감지를 위한 ContactListner를 world에 설정

}

void Physics::Update(float deltaTime)
{
	if (!world) return;

	// 물리 세계에서 한 프레임만큼 물리 시뮬레이션을 진행
	world->Step(deltaTime, WORLD_VELOCITY_ITERATION, WORLD_POSITION_ITERATION); // deltaTime 경과에 따른 속도와 위치 업데이트 (속도 및 위치 계산을 위한 연산 횟수)
	
	// Step 이후 삭제 대기 리스트 처리 (step중 destroy 할 수 없기 때문에 이렇게 처리)
	for (b2Body* body : bodiesToDestroy) 
	{
		// body->GetUserData().pointer 에 동적할당된 객체도 delete 해줘야 하지 않을까? (스마트 포인터 고려)
		if (!body) continue;
		Physics::world->DestroyBody(body);
	}
	bodiesToDestroy.clear();

	for (pair<b2Body*, bool> bodyInfo : bodiesSetEnabled)
	{
		if (!bodyInfo.first) continue;
		bodyInfo.first->SetEnabled(bodyInfo.second);
	}

	bodiesSetEnabled.clear();
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

void Physics::Shutdown()
{
	if (!world) return;

	cout << "Destroying Physics world..." << endl;

	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			if (fixture->GetUserData().pointer) fixture->GetUserData().pointer = 0; // Dangling pointer 방지
		}
	}

	delete world;
	world = nullptr;
	fixtureDataList.clear(); // 모든 FixtureData 소멸 (unique_ptr로 관리되던 객체가 안전하게 메모리 해제됨)
}

void Physics::AddFixtureData(unique_ptr<FixtureData> fixtureData)
{
	fixtureDataList.push_back(move(fixtureData)); // fixtureData 소유권을 fixtureDataList로 이전
}
