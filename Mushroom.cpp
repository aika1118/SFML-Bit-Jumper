#include "Mushroom.h"
#include "Game.h"

void Mushroom::Begin()
{
	_tag = "mushroom";

    // body 정의 후 wordl에 추가
    b2BodyDef bodyDef;
    bodyDef.position.Set(_position.x, _position.y);
    _body = Physics::world->CreateBody(&bodyDef);

    // collider 정의
    b2PolygonShape shape;
    shape.SetAsBox(OBJECT_MUSHROOM_SIZE / 2.f, OBJECT_MUSHROOM_SIZE / 2.f);

    // fixtureData 정의
    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;

    // fixture 정의 후 body 연결
    b2FixtureDef fixtureDef;
    fixtureDef.userData.pointer = (uintptr_t)fixtureData;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;
    _body->CreateFixture(&fixtureDef);

    // unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
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

// 점프 상승 효과중일 때 캐릭터 렌더링을 구분하여 player가 인지할 수 있도록 처리 필요
void Mushroom::Render(Renderer& renderer)
{
    if (Game::getInstance().getPlayer().bIsJumpImproved == false) // 점프 상승 효과 있으면 렌더링 하지 않음 (버섯 먹은 후 일정시간 지나면 다시 버섯이 보임)
        renderer.Draw(Resources::_textures["mushroom.png"], _position, Vector2f(OBJECT_MUSHROOM_SIZE, OBJECT_MUSHROOM_SIZE));
}
