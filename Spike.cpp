#include "Spike.h"

void Spike::Begin()
{
    _tag = "spike";

    b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    b2Body* body = Physics::world->CreateBody(&bodyDef);

    vector<b2Vec2> vs(4);
    vs[0].Set(-OBJECT_SPIKE_SIZE / 2.f, -OBJECT_SPIKE_SIZE / 2.f);
    vs[1].Set(+OBJECT_SPIKE_SIZE / 2.f, -OBJECT_SPIKE_SIZE / 2.f);
    vs[2].Set(+OBJECT_SPIKE_SIZE / 2.f, +OBJECT_SPIKE_SIZE / 2.f);
    vs[3].Set(-OBJECT_SPIKE_SIZE / 2.f, +OBJECT_SPIKE_SIZE / 2.f);

    b2ChainShape chain;
    chain.CreateLoop(&vs[0], 4);

    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;

    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &chain;
    body->CreateFixture(&fixtureDef);

    // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
    unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
    Physics::AddFixtureData(move(fixtureDataPtr));
}

void Spike::Update(float deltaTime)
{
	
}

void Spike::Render(Renderer& renderer)
{
	renderer.Draw(Resources::_textures["spikes.png"], _position, Vector2f(OBJECT_SPIKE_SIZE, OBJECT_SPIKE_SIZE));
}
