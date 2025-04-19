#include "Enemy.h"
#include "Game.h" // 순환참조 회피

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

	// 기존 _body는 Physics::Init()에서 초기화됨

	if (!Physics::world) return;
	
	// 현재 world에 맞게 새로운 _body 생성

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // 물리적 힘에 의해 움직이는 물체
	bodyDef.position.Set(_position.x, _position.y);
	bodyDef.fixedRotation = true; // 회전하지 않음
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

	if (_isEnemyAttacked) // 스킬에 피격당한 경우 일정 시간 멈춤
	{
		_hitTimer += deltaTime;

		if (_hitTimer >= ENEMY_STOP_TIME_AFTER_ATTACKED) // 멈추는 시간 지난 경우 관련 변수 초기화
		{
			_isEnemyAttacked = false;
			_hitTimer = 0.f;
		}
		else
			velocity.x = 0.f;
	}
	else // 스킬에 피격당하지 않은 경우
	{
		_animation.Update(deltaTime);
		if (abs(velocity.x) <= 0.02f) // 벽등에 부딪혀 속도가 0에 가까워지면 속도 방향을 바꿈
			_movement *= -1.f;

		velocity.x = _movement;

		if (velocity.x < -0.02f)
			_facingLeft = true;
		else if (velocity.x > 0.02f)
			_facingLeft = false;
	}
	

	_body->SetLinearVelocity(velocity);
	_position = Vector2f(_body->GetPosition().x, _body->GetPosition().y); // 물리적 위치 몸체와 위치 동기화
	_angle = _body->GetAngle() * (180.f / M_PI);
}

void Enemy::Render(Renderer& renderer)
{
	if (!_isActive) return;
	// _isEnemyAttacked = true 일 때 적 피격 animation 별도 구축 필요

	// _facingLeft : 캐릭터 속도 방향에 따라 x축 렌더링 방향 결정
	// _position : 캐릭터 죽었을 때 y축 위치 재조정
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
	Game::getInstance().GetEnemyPool()->ScheduleEnemy(this, ENEMY_RESPAWN_TIME); // N초 뒤 리스폰 설정
}


