#include "Enemy.h"
#include "Game.h" // 순환참조 회피

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
			Game::getInstance().DeleteObject(this); // 더이상 렌더링 되지 않도록 삭제 (물리적 몸체는 적이 죽을 때 바로 이미 제거됨)

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
	// _facingLeft : 캐릭터 속도 방향에 따라 x축 렌더링 방향 결정
	// _position : 캐릭터 죽었을 때 y축 위치 재조정
	renderer.Draw(_animation.GetTexture(), 
		!_isDead ? _position : Vector2f(_position.x, _position.y + ENEMY_RADIUS), 
		Vector2f(_facingLeft ? 1.f : -1.f, _isDead ? -0.2f : 1.f),
		_angle
	);
}

void Enemy::destroyBody()
{
	Object::destroyBody(); // 부모 함수 호출
	_isDead = true;
}

bool Enemy::IsDead()
{
	return _isDead;
}


