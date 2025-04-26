#include "Physics.h"

b2World* Physics::world; // static ����
b2Draw* Physics::debugDraw; // static ����
set<b2Body*> Physics::bodiesToDestroy; // static ����
set<pair<b2Body*, bool>> Physics::bodiesSetEnabled;
vector<unique_ptr<FixtureData>> Physics::fixtureDataList;

void Physics::Init()
{
	if (world)
		delete world; // ���� world �ʱ�ȭ (b2Body ���� ���ҽ��� �����ϰ� ����)

	bodiesToDestroy.clear();
	bodiesSetEnabled.clear();
	fixtureDataList.clear();

	world = new b2World(b2Vec2(WORLD_GRAVITY_X, WORLD_GRAVITY_Y));
	world->SetDebugDraw(debugDraw);
	world->SetContactListener(new MyGlobalContactListener()); // �浹 ������ ���� ContactListner�� world�� ����

}

void Physics::Update(float deltaTime)
{
	if (!world) return;

	// ���� ���迡�� �� �����Ӹ�ŭ ���� �ùķ��̼��� ����
	world->Step(deltaTime, WORLD_VELOCITY_ITERATION, WORLD_POSITION_ITERATION); // deltaTime ����� ���� �ӵ��� ��ġ ������Ʈ (�ӵ� �� ��ġ ����� ���� ���� Ƚ��)
	
	// Step ���� ���� ��� ����Ʈ ó�� (step�� destroy �� �� ���� ������ �̷��� ó��)
	for (b2Body* body : bodiesToDestroy) 
	{
		// body->GetUserData().pointer �� �����Ҵ�� ��ü�� delete ����� ���� ������? (����Ʈ ������ ���)
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
		//debugDraw->SetFlags(b2Draw::e_aabbBit); // ��ü�� �����ϴ� ���簢�� ��� ����
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
			if (fixture->GetUserData().pointer) fixture->GetUserData().pointer = 0; // Dangling pointer ����
		}
	}

	delete world;
	world = nullptr;
	fixtureDataList.clear(); // ��� FixtureData �Ҹ� (unique_ptr�� �����Ǵ� ��ü�� �����ϰ� �޸� ������)
}

void Physics::AddFixtureData(unique_ptr<FixtureData> fixtureData)
{
	fixtureDataList.push_back(move(fixtureData)); // fixtureData �������� fixtureDataList�� ����
}
