#include "BoxFragile.h"

void BoxFragile::Begin()
{
    _tag = "box_fragile";

    /*b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    b2Body* body = Physics::world->CreateBody(&bodyDef);

    vector<b2Vec2> vs(4);
    vs[0].Set(-OBJECT_BOX_FRAGILE_SIZE / 2.f, -OBJECT_BOX_FRAGILE_SIZE / 2.f);
    vs[1].Set(+OBJECT_BOX_FRAGILE_SIZE / 2.f, -OBJECT_BOX_FRAGILE_SIZE / 2.f);
    vs[2].Set(+OBJECT_BOX_FRAGILE_SIZE / 2.f, +OBJECT_BOX_FRAGILE_SIZE / 2.f);
    vs[3].Set(-OBJECT_BOX_FRAGILE_SIZE / 2.f, +OBJECT_BOX_FRAGILE_SIZE / 2.f);

    b2ChainShape chain;
    chain.CreateLoop(&vs[0], 4);

    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;

    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &chain;
    body->CreateFixture(&fixtureDef);*/

    
    // body 정의 후 wordl에 추가
    b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    _body = Physics::world->CreateBody(&bodyDef);

    // collider 정의
    b2PolygonShape shape;
    shape.SetAsBox(OBJECT_BOX_FRAGILE_SIZE / 2.f, OBJECT_BOX_FRAGILE_SIZE / 2.f);

    // fixtureData 정의
    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;
    fixtureData->canJump = true;

    // fixture 정의 후 body 연결
    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;
    _body->CreateFixture(&fixtureDef);
    
}

void BoxFragile::Update(float deltaTime)
{
    
}

void BoxFragile::Render(Renderer& renderer)
{
    renderer.Draw(GetTextureBasedOnHP(), _position, Vector2f(OBJECT_BOX_FRAGILE_SIZE, OBJECT_BOX_FRAGILE_SIZE));
}

void BoxFragile::TakeDamage()
{
	_hp--;
}

int BoxFragile::getHP()
{
    return _hp;
}

Texture BoxFragile::GetTextureBasedOnHP() const
{
    if (_hp < 0) return Texture();

    return Resources::_textures["box_fragile" + to_string(_hp) + ".png"];
}
