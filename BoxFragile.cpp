#include "BoxFragile.h"

void BoxFragile::Begin()
{
    _tag = "box_fragile";
 
    // body ���� �� wordl�� �߰�
    b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    _body = Physics::world->CreateBody(&bodyDef);

    // collider ����
    b2PolygonShape shape;
    shape.SetAsBox(OBJECT_BOX_FRAGILE_SIZE / 2.f, OBJECT_BOX_FRAGILE_SIZE / 2.f);

    // fixtureData ����
    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;
    fixtureData->canJump = true;

    // fixture ���� �� body ����
    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;
    _body->CreateFixture(&fixtureDef);

    // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
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
