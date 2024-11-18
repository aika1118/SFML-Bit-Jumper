#include "Player.h"

void Player::Begin()
{
	b2BodyDef bodyDef; // 물리적 몸체 정의
	b2FixtureDef fixtureDef;

	bodyDef.type = b2_dynamicBody;  // 물체가 물리 시뮬레이션에서 중력, 충돌, 힘 등과 같은 외부 요인의 영향을 받음
	bodyDef.position.Set(_position.x, _position.y);
	bodyDef.fixedRotation = true; // 물체가 회전하지 않도록 설정
	body = Physics::world->CreateBody(&bodyDef); // body = 새로 생성된 몸체에 대해 포인터로 참조가능

	_fixtureData.listener = this;
	_fixtureData.player = this;
	_fixtureData.type = FixtureDataType::Player;

	
	fixtureDef.userData.pointer = (uintptr_t)&_fixtureData; // 차후 Contact 상황 등에서 fixture를 통해 fixtureData 접근 가능
	fixtureDef.density = 1.f; // 밀도
	fixtureDef.friction = 0.f; // 마찰

	// 캐릭터를 감싸는 충돌체
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.5f, PLAYER_NORMALIZED_HEIGHT / 2.f);
	fixtureDef.shape = &polygonShape;
	body->CreateFixture(&fixtureDef);

	// 캐릭터 발 밑에 설치할 sensor (땅에 닿을 때 점프 처리를 위한 sensor)

	b2FixtureDef sensorFixtureDef;
	b2PolygonShape sensorPolygonShape;

	_sensorFixtureData.listener = this;
	_sensorFixtureData.player = this;
	_sensorFixtureData.type = FixtureDataType::PlayerSensor;

	sensorFixtureDef.userData.pointer = (uintptr_t)&_sensorFixtureData; // 차후 Contact 상황 등에서 fixture를 통해 fixtureData 접근 가능
	sensorPolygonShape.SetAsBox(PLAYER_SENSOR_HX, PLAYER_SENSOR_HY, b2Vec2(0.f, PLAYER_NORMALIZED_HEIGHT / 2.f), 0.f);
	sensorFixtureDef.isSensor = true; // sensor : 충돌을 감지하지만 물리적인 상호작용은 하지 않는 특수한 fixture
	sensorFixtureDef.shape = &sensorPolygonShape;

	body->CreateFixture(&sensorFixtureDef);
}

void Player::Update(float deltaTime)
{
	_position = Vector2f(body->GetPosition().x, body->GetPosition().y); // Physics::Update()에서 world.Step에 의해 프레임 당 물리 계산 적용중, 이로 인해 body position 변화 발생
	_angle = body->GetAngle() * (180.f / M_PI); // 라디안에서 도(degree) 변환

	float move = PLAYER_MOVE_VELOCITY;

	if (Keyboard::isKeyPressed(Keyboard::LShift)) // shift 키다운 시 이동속도 2배
		move *= 2;

	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x = 0.f;

	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x += move;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x -= move;

	// PLAYER_MAX_JUMP_COUNT 횟수만큼 점프 가능하도록 구현 (최초 점프는 땅에서만 가능)

	bool currentSpaceState = Keyboard::isKeyPressed(Keyboard::Space); // 프레임 별로 keyDown, keyReleased 판별을 위한 상태변수

	// KeyDown: Space 키가 눌렸고, 이전 프레임에서는 떼어져 있었다면
	if (currentSpaceState && !_previousSpaceState) 
	{
		if (_groundContactCount > 0) // 최초 땅에서 점프하는 순간
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
		}

		else if (1 <= _jumpCount && _jumpCount < PLAYER_MAX_JUMP_COUNT) // 이미 점프한 후 연속 점프 시도 상황
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
		}
	}

	// KeyReleased: Space 키가 떼어졌을 때
	if (!currentSpaceState && _previousSpaceState) {
		if (_isJumping) // space 키다운으로 유효한 점프 상황에서 key released 되었을 때 (그냥 땅에서 떨어지며 공중에서 점프하는 것 방지)
		{
			++_jumpCount;
			_isJumping = false;
		}
	}

	_previousSpaceState = currentSpaceState;
	
	
	if (velocity.x < -0.02f)
		_facingLeft = true;
	else if (velocity.x > 0.02f)
		_facingLeft = false;

	body->SetLinearVelocity(velocity);
}

void Player::Draw(Renderer& renderer)
{
	renderer.Draw(Resources::_textures["idle.png"], _position, Vector2f(_facingLeft ? -1.f : 1.f, PLAYER_NORMALIZED_HEIGHT), _angle); // 캐릭터가 왼쪽 바라보면 size.x에 음수 적용
}

void Player::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::MapTile) // player가 mapTile과 닿아있는 경우
	{
		++_groundContactCount; 

		// jump 상태 초기화
		_jumpCount = 0; 
		_isJumping = false;

		return;
	}

}

void Player::OnEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::MapTile) // player가 mapTile과 멀어지는 경우
	{
		_groundContactCount = _groundContactCount > 0 ? --_groundContactCount : _groundContactCount;
		return;
	}
}