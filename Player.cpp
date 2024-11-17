#include "Player.h"

void Player::Begin()
{
	b2BodyDef bodyDef; // ������ ��ü ����
	b2FixtureDef fixtureDef;

	bodyDef.type = b2_dynamicBody;  // ��ü�� ���� �ùķ��̼ǿ��� �߷�, �浹, �� ��� ���� �ܺ� ������ ������ ����
	bodyDef.position.Set(_position.x, _position.y);
	bodyDef.fixedRotation = true; // ��ü�� ȸ������ �ʵ��� ����
	body = Physics::world->CreateBody(&bodyDef); // body = ���� ������ ��ü�� ���� �����ͷ� ��������

	_fixtureData.listener = this;
	_fixtureData.player = this;
	_fixtureData.type = FixtureDataType::Player;

	
	fixtureDef.userData.pointer = (uintptr_t)&_fixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
	fixtureDef.density = 1.f; // �е�
	fixtureDef.friction = 0.f; // ����

	// ĳ���͸� ���δ� �浹ü
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.5f, PLAYER_NORMALIZED_HEIGHT / 2.f);
	fixtureDef.shape = &polygonShape;
	body->CreateFixture(&fixtureDef);

	// ĳ���� �� �ؿ� ��ġ�� sensor (���� ���� �� ���� ó���� ���� sensor)

	b2FixtureDef sensorFixtureDef;
	b2PolygonShape sensorPolygonShape;

	_sensorFixtureData.listener = this;
	_sensorFixtureData.player = this;
	_sensorFixtureData.type = FixtureDataType::PlayerSensor;

	sensorFixtureDef.userData.pointer = (uintptr_t)&_sensorFixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
	sensorPolygonShape.SetAsBox(PLAYER_SENSOR_HX, PLAYER_SENSOR_HY, b2Vec2(0.f, PLAYER_NORMALIZED_HEIGHT / 2.f), 0.f);
	sensorFixtureDef.isSensor = true; // sensor : �浹�� ���������� �������� ��ȣ�ۿ��� ���� �ʴ� Ư���� fixture
	sensorFixtureDef.shape = &sensorPolygonShape;

	body->CreateFixture(&sensorFixtureDef);
}

void Player::Update(float deltaTime)
{
	cout << _jumpCount << endl;

	_position = Vector2f(body->GetPosition().x, body->GetPosition().y); // Physics::Update()���� world.Step�� ���� ������ �� ���� ��� ������, �̷� ���� body position ��ȭ �߻�
	_angle = body->GetAngle() * (180.f / M_PI); // ���ȿ��� ��(degree) ��ȯ

	float move = PLAYER_MOVE_VELOCITY;

	if (Keyboard::isKeyPressed(Keyboard::LShift)) // shift Ű�ٿ� �� �̵��ӵ� 2��
		move *= 2;

	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x = 0.f;

	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x += move;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x -= move;

	if (Keyboard::isKeyPressed(Keyboard::Space) && canJump())
	{
		velocity.y = PLAYER_JUMP_VELOCITY;
	}
	
	if (velocity.x < -0.02f)
		_facingLeft = true;
	else if (velocity.x > 0.02f)
		_facingLeft = false;

	body->SetLinearVelocity(velocity);
}

void Player::Draw(Renderer& renderer)
{
	renderer.Draw(Resources::_textures["idle.png"], _position, Vector2f(_facingLeft ? -1.f : 1.f, PLAYER_NORMALIZED_HEIGHT), _angle); // ĳ���Ͱ� ���� �ٶ󺸸� size.x�� ���� ����
}

void Player::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::MapTile) // player�� mapTile�� ����ִ� ���
	{
		++_groundContactCount;
		return;
	}

}

void Player::OnEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::MapTile) // player�� mapTile�� �־����� ���
	{
		_groundContactCount = _groundContactCount > 0 ? --_groundContactCount : _groundContactCount;
		return;
	}
}

bool Player::canJump()
{
	return _groundContactCount > 0;
}
