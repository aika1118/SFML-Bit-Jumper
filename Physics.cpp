#include "Physics.h"

b2World* Physics::world; // static ����
b2Draw* Physics::debugDraw; // static ����
vector<b2Body*> Physics::bodiesToDestroy; // static ����

void Physics::Init()
{
	if (world)
		delete world; // ���� world �ʱ�ȭ

	world = new b2World(b2Vec2(WORLD_GRAVITY_X, WORLD_GRAVITY_Y));
	world->SetDebugDraw(debugDraw);
	world->SetContactListener(new MyGlobalContactListener()); // �浹 ������ ���� ContactListner�� world�� ����

}

void Physics::Update(float deltaTime)
{
	// ���� ���迡�� �� �����Ӹ�ŭ ���� �ùķ��̼��� ����
	world->Step(deltaTime, WORLD_VELOCITY_ITERATION, WORLD_POSITION_ITERATION); // deltaTime ����� ���� �ӵ��� ��ġ ������Ʈ (�ӵ� �� ��ġ ����� ���� ���� Ƚ��)
	
	// Step ���� ���� ��� ����Ʈ ó�� (step�� destroy �� �� ���� ������ �̷��� ó��)
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
		//debugDraw->SetFlags(b2Draw::e_aabbBit); // ��ü�� �����ϴ� ���簢�� ��� ����
		world->SetDebugDraw(debugDraw);
	}

	world->DebugDraw();
}
