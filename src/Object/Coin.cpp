#include "Coin.h"

void Coin::Begin()
{
	_tag = "coin"; // object를 tag로 구분

	_animation = Animation(0.9f,
		{
			AnimFrame(0.1f, Resources::_textures["coin_01.png"]),
			AnimFrame(0.2f, Resources::_textures["coin_02.png"]),
			AnimFrame(0.3f, Resources::_textures["coin_03.png"]),
			AnimFrame(0.4f, Resources::_textures["coin_04.png"]),
			AnimFrame(0.5f, Resources::_textures["coin_05.png"]),
			AnimFrame(0.6f, Resources::_textures["coin_06.png"]),
			AnimFrame(0.7f, Resources::_textures["coin_07.png"]),
			AnimFrame(0.8f, Resources::_textures["coin_08.png"])
		});

	// body 정의 후 wordl에 추가
	b2BodyDef bodyDef;
	bodyDef.position.Set(_position.x, _position.y);
	_body = Physics::world->CreateBody(&bodyDef);
	
	// collider 정의
	b2PolygonShape shape;
	shape.SetAsBox(OBJECT_COIN_SIZE / 2.f, OBJECT_COIN_SIZE / 2.f);

	// fixtureData 정의
	FixtureData* fixtureData = new FixtureData();
	fixtureData->type = FixtureDataType::Object;
	fixtureData->object = this;

	// fixture 정의 후 body 연결
	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = (uintptr_t)fixtureData;
	fixtureDef.isSensor = true;
	fixtureDef.density = 0.f;
	fixtureDef.shape = &shape;
	_body->CreateFixture(&fixtureDef);
	
	// unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
	unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
	Physics::AddFixtureData(move(fixtureDataPtr));
}

void Coin::Update(float deltaTime)
{
	_animation.Update(deltaTime);
}

void Coin::Render(Renderer& renderer)
{
	renderer.Draw(_animation.GetTexture(), _position, Vector2f(OBJECT_COIN_SIZE, OBJECT_COIN_SIZE));
}
