#include "Enemy.h"
#include "Game.h" // ��ȯ���� ȸ��

void Enemy::Begin()
{
	_movement = ENEMY_MOVE_VELOCITY;
	_isDead = false;
	_destroyTimer = 0.f;
	_facingLeft = false;
	_isActive = false;
	_hp = 2;
	_hitTimer = 0.f;
	_isEnemyAttacked = false;

	_animation = Animation(0.45f,
		{
			AnimFrame(0.15f, Resources::_textures["slime_normal.png"]),
			AnimFrame(0.3f, Resources::_textures["slime_walk.png"])
		});

	_tag = "enemy";

	_fixtureData.object = this;
	_fixtureData.type = FixtureDataType::Object;

	// ���� _body�� Physics::Init()���� �ʱ�ȭ��

	if (!Physics::world) return;
	
	// ���� world�� �°� ���ο� _body ����

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

}

void Enemy::Update(float deltaTime)
{
	if (!_isActive) return;

	if (_isDead)
	{
		_destroyTimer += deltaTime;
		if (_destroyTimer >= ENEMY_DESTROY_TIME) _isActive = false;

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
	if (!_isActive) return;
	// _isEnemyAttacked = true �� �� �� �ǰ� animation ���� ���� �ʿ�

	// _facingLeft : ĳ���� �ӵ� ���⿡ ���� x�� ������ ���� ����
	// _position : ĳ���� �׾��� �� y�� ��ġ ������
	renderer.Draw(_animation.GetTexture(), 
		!_isDead ? _position : Vector2f(_position.x, _position.y + ENEMY_RADIUS), 
		Vector2f(_facingLeft ? 1.f : -1.f, _isDead ? -0.2f : 1.f),
		_angle
	);
}

bool Enemy::IsDead()
{
	return _isDead;
}

void Enemy::SetActive(bool active)
{
	if (!_body) return;

	_isActive = active;
	Physics::bodiesSetEnabled.insert({_body, active});
}

bool Enemy::IsActive() const
{
	return _isActive;
}

void Enemy::Reset(Vector2f position)
{
	_spawnPosition = position;
	_position = position;
	_isDead = false;
	_destroyTimer = 0.f;
	_hp = 2;
	_hitTimer = 0.f;
	_isEnemyAttacked = false;
	_movement = ENEMY_MOVE_VELOCITY;
	_facingLeft = false;
	
	_body->SetTransform(b2Vec2(position.x, position.y), 0.f);
	_body->SetLinearVelocity(b2Vec2(0.f, 0.f));
	Physics::bodiesSetEnabled.insert({ _body, true });
}

Vector2f Enemy::GetSpawnPosition() const
{
	return _spawnPosition;
}

void Enemy::SetDead()
{
	_isDead = true;
	//_isActive = false;
	Physics::bodiesSetEnabled.insert({ _body, false });
	Game::getInstance().GetEnemyPool()->ScheduleEnemy(this, ENEMY_RESPAWN_TIME); // N�� �� ������ ����
}


