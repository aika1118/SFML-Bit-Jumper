#include "Mushroom.h"
#include "Game.h"

void Mushroom::Begin()
{
	_tag = "mushroom";

    // body ���� �� wordl�� �߰�
    b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    _body = Physics::world->CreateBody(&bodyDef);

    // collider ����
    b2PolygonShape shape;
    shape.SetAsBox(OBJECT_MUSHROOM_SIZE / 2.f, OBJECT_MUSHROOM_SIZE / 2.f);

    // fixtureData ����
    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;

    // fixture ���� �� body ����
    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    _body->CreateFixture(&fixtureDef);

    // unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
    unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
    Physics::AddFixtureData(move(fixtureDataPtr));
}

void Mushroom::Update(float deltaTime)
{
    if (Game::getInstance().getPlayer().bIsJumpImproved == true)
    {
        _timer += deltaTime;
        if (_timer >= _jumpImproveTimeLength)
        {
            Game::getInstance().getPlayer().bIsJumpImproved = false;
            _timer = 0.f;
            cout << "jump improve off" << endl;
        }
    }
}

// ���� ��� ȿ������ �� ĳ���� �������� �����Ͽ� player�� ������ �� �ֵ��� ó�� �ʿ�
void Mushroom::Render(Renderer& renderer)
{
    if (Game::getInstance().getPlayer().bIsJumpImproved == false) // ���� ��� ȿ�� ������ ������ ���� ���� (���� ���� �� �����ð� ������ �ٽ� ������ ����)
        renderer.Draw(Resources::_textures["mushroom.png"], _position, Vector2f(OBJECT_MUSHROOM_SIZE, OBJECT_MUSHROOM_SIZE));
}
