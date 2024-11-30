#include "Enemy.h"
#include "Game.h" // ��ȯ���� ȸ��

void Enemy::Begin()
{
	_movement = ENEMY_MOVE_VELOCITY;
	_isDead = false;
	_destroyTimer = 0.f;
	_facingLeft = false;

	_animation = Animation(0.45f,
		{
			AnimFrame(0.15f, Resources::_textures["slime_normal.png"]),
			AnimFrame(0.3f, Resources::_textures["slime_walk.png"])
		});

	_tag = "enemy";

	_fixtureData.object = this;
	_fixtureData.type = FixtureDataType::Object;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // ������ ���� ���� �����̴� ��ü
	bodyDef.position.Set(_position.x, _position.y);
	bodyDef.fixedRotation = true; // ȸ������ ����
	_body = Physics::world->CreateBody(&bodyDef);

	b2CircleShape circleShape;
	circleShape.m_radius = ENEMY_RADIUS;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = (uintptr_t)&_fixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.f;
	_body->CreateFixture(&fixtureDef);

	_hp = 2;
	_hitTimer = 0.f;
	_isEnemyAttacked = false;
}

void Enemy::Update(float deltaTime)
{
	if (_isDead)
	{
		_destroyTimer += deltaTime;
		if (_destroyTimer >= ENEMY_DESTROY_TIME)
			Game::getInstance().DeleteObject(this); // ���̻� ������ ���� �ʵ��� ���� (������ ��ü�� ���� ���� �� �ٷ� �̹� ���ŵ�)

		return;
	}

	b2Vec2 velocity = _body->GetLinearVelocity();

	if (_isEnemyAttacked) // ��ų�� �ǰݴ��� ��� ���� �ð� ����
	{
		_hitTimer += deltaTime;

		if (_hitTimer >= ENEMY_STOP_TIME_AFTER_ATTACKED) // ���ߴ� �ð� ���� ��� ���� ���� �ʱ�ȭ
		{
			_isEnemyAttacked = false;
			_hitTimer = 0.f;
		}
		else
			velocity.x = 0.f;
	}
	else // ��ų�� �ǰݴ����� ���� ���
	{
		_animation.Update(deltaTime);
		if (abs(velocity.x) <= 0.02f) // ��� �ε��� �ӵ��� 0�� ��������� �ӵ� ������ �ٲ�
			_movement *= -1.f;

		velocity.x = _movement;

		if (velocity.x < -0.02f)
			_facingLeft = true;
		else if (velocity.x > 0.02f)
			_facingLeft = false;
	}
	

	_body->SetLinearVelocity(velocity);
	_position = Vector2f(_body->GetPosition().x, _body->GetPosition().y); // ������ ��ġ ��ü�� ��ġ ����ȭ
	_angle = _body->GetAngle() * (180.f / M_PI);
}

void Enemy::Render(Renderer& renderer)
{
	// _facingLeft : ĳ���� �ӵ� ���⿡ ���� x�� ������ ���� ����
	// _position : ĳ���� �׾��� �� y�� ��ġ ������
	renderer.Draw(_animation.GetTexture(), 
		!_isDead ? _position : Vector2f(_position.x, _position.y + ENEMY_RADIUS), 
		Vector2f(_facingLeft ? 1.f : -1.f, _isDead ? -0.2f : 1.f),
		_angle
	);
}

void Enemy::destroyBody()
{
	Object::destroyBody(); // �θ� �Լ� ȣ��
	_isDead = true;
}

bool Enemy::IsDead()
{
	return _isDead;
}


