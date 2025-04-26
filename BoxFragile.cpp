#include "BoxFragile.h"

void BoxFragile::Begin()
{
    _tag = "box_fragile";
 
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

    // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
    unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
    Physics::AddFixtureData(move(fixtureDataPtr));
    
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
