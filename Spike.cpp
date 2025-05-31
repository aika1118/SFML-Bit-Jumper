#include "Spike.h"

void Spike::Begin()
{
    _tag = "spike";

    // body ���� �� wordl�� �߰�
    b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    b2Body* body = Physics::world->CreateBody(&bodyDef);

    // collider ����
    vector<b2Vec2> vs(4);
    vs[0].Set(-OBJECT_SPIKE_SIZE / 2.f, -OBJECT_SPIKE_SIZE / 2.f);
    vs[1].Set(+OBJECT_SPIKE_SIZE / 2.f, -OBJECT_SPIKE_SIZE / 2.f);
    vs[2].Set(+OBJECT_SPIKE_SIZE / 2.f, +OBJECT_SPIKE_SIZE / 2.f);
    vs[3].Set(-OBJECT_SPIKE_SIZE / 2.f, +OBJECT_SPIKE_SIZE / 2.f);

    b2ChainShape chain;
    chain.CreateLoop(&vs[0], 4);

    // fixtureData ����
    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;

    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &chain;
    body->CreateFixture(&fixtureDef);

    // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
    unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
    Physics::AddFixtureData(move(fixtureDataPtr));
}

void Spike::Update(float deltaTime)
{
	// Update �ʿ� �� ����
}

void Spike::Render(Renderer& renderer)
{
	renderer.Draw(Resources::_textures["spikes.png"], _position, Vector2f(OBJECT_SPIKE_SIZE, OBJECT_SPIKE_SIZE));
}
