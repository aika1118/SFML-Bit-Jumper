#include "Player.h"
#include "Game.h" // ��ȯ���� ȸ��

void Player::Begin()
{
	// Animation Frame ����
	_runAnimation = Animation(1.1f,
		{
			AnimFrame(0.1f, Resources::_textures["walk0001.png"]),
			AnimFrame(0.2f, Resources::_textures["walk0002.png"]),
			AnimFrame(0.3f, Resources::_textures["walk0003.png"]),
			AnimFrame(0.4f, Resources::_textures["walk0004.png"]),
			AnimFrame(0.5f, Resources::_textures["walk0005.png"]),
			AnimFrame(0.6f, Resources::_textures["walk0006.png"]),
			AnimFrame(0.7f, Resources::_textures["walk0007.png"]),
			AnimFrame(0.8f, Resources::_textures["walk0008.png"]),
			AnimFrame(0.9f, Resources::_textures["walk0009.png"]),
			AnimFrame(1.0f, Resources::_textures["walk0010.png"])
		});


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

	// �̿� ����Լ� �ʱ�ȭ
	_hp = 3;
	_isDead = false;
	_playerStatus = PlayerStatus::Idle;
}

void Player::Update(float deltaTime)
{
	_position = Vector2f(body->GetPosition().x, body->GetPosition().y); // Physics::Update()���� world.Step�� ���� ������ �� ���� ��� ������, �̷� ���� body position ��ȭ �߻�
	_angle = body->GetAngle() * (180.f / M_PI); // ���ȿ��� ��(degree) ��ȯ
	b2Vec2 velocity = body->GetLinearVelocity();
	
	HandleSkill(deltaTime, velocity); // Ű���� �Է¿� ���� skill ó��
	HandleMove(deltaTime, velocity); // Ű���� �Է¿� ���� move ó��
	HandleJump(velocity); // Ű���� �Է¿� ���� jump ó��
	body->SetLinearVelocity(velocity);
}

void Player::Draw(Renderer& renderer)
{
	if (_playerStatus == PlayerStatus::Attacking) // ��ų ��� ���� ���� ��ų ������
	{
		SkillManager::getInstance().Render(renderer, _currentSkillId);
		return;
	}

	renderer.Draw(_textureToDraw, _position, Vector2f(_facingLeft ? -1.f : 1.f, PLAYER_NORMALIZED_HEIGHT), _angle); // ĳ���Ͱ� ���� �ٶ󺸸� size.x�� ���� ����
}

Player::~Player()
{
}

void Player::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	// player�� mapTile�� ����ִ� ���
	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::MapTile) 
	{
		++_groundContactCount; 

		// jump ���� �ʱ�ȭ
		_jumpCount = 0; 
		_isJumping = false;

		return;
	}

	// player�� coin�� �浹�� ���
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::Object && otherData->object->_tag == "coin")
	{
		Coin* coin = dynamic_cast<Coin*>(otherData->object); // �����ϰ� �ٿ� ĳ���� (otherData->object�� Coin*�� �ƴҰ�� nullptr ��ȯ)
		if (!coin)
			return;
		
		coin->destroyBody(); // world���� body�� destroy (��Ȯ���� ���� ��⸮��Ʈ���� �ϴ� �߰� �� Physics���� step ��� �� �ϰ������� destroyBody ����)
		Game::getInstance().DeleteObject(otherData->object); // object�� destroy�Ͽ� ���̻� �ش� object�� ���� update, render���� �ʵ��� ��

		++_coins;

		cout << "coins = " << _coins << endl;

		return;
	}

	// player�� ���� �浹�� ���
	if (otherData->type == FixtureDataType::Object && otherData->object->_tag == "enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(otherData->object);
		if (!enemy)
			return;

		// player �� �� ������ ���� �浹�� ��� ������ ��ü�� ���� destroy
		// texture�� �����ð� ���� ���� ������� ��� �׸� (���� objects���� delete ó�� �� ����)
		if (selfData->type == FixtureDataType::PlayerSensor)
			enemy->destroyBody();

		// enemy�� ����ְ� player�� ���� �Ϲ������� �浹�� ��� �ǰ�ó��
		else if (enemy->IsDead() == false)
		{
			_hp = max(0, _hp - 1);
			cout << _hp << endl;

			if (_hp <= 0)
				_isDead = true;
		}

		return;
	}

	// player�� save Ÿ�ϰ� �浹�� ���
	// �ֱٿ� �浹�� save Ÿ�Ͽ� ������ ��ġ �����ϴ� �۾�
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::SaveTile)
	{
		int preSaveX = Game::getInstance()._savePositionX;
		int preSaveY = Game::getInstance()._savePositionY;
		int curSaveX = otherData->mapX;
		int curSaveY = otherData->mapY;
		
		// ������ ���̺��� ��ҿ� ������ ��� �ٷ� return ó��
		if (preSaveX == curSaveX && preSaveY == curSaveY)
			return;

		// ���� ���̺� ����� texture�� save_used.png�� ����
		Map::getInstance()._grid[curSaveX][curSaveY] = &Resources::_textures["save_used.png"];

		// ���� ���̺� ����� �ִ� ��� ���� ���̺� ����� �������� ��������
		if (preSaveX || preSaveY)
			Map::getInstance()._grid[preSaveX][preSaveY] = &Resources::_textures["save.png"];

		// �ֱ� ���̺� ��Ҹ� Game Ŭ������ ���
		Game::getInstance()._savePositionX = curSaveX;
		Game::getInstance()._savePositionY = curSaveY;
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

void Player::HandleMove(float deltaTime, b2Vec2& velocity)
{
	if (_playerStatus == PlayerStatus::Attacking) // skill ��� ���̸� return
		return;

	float move = PLAYER_MOVE_VELOCITY;

	if (Keyboard::isKeyPressed(Keyboard::LShift)) // shift Ű�ٿ� �� �̵��ӵ� 2��
		move *= 2;

	
	velocity.x = 0.f;

	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x = +move;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x = -move;

	// ���� time�� ���� draw�� texture ���
	_runAnimation.Update(deltaTime);
	_textureToDraw = _runAnimation.GetTexture();

	if (velocity.x < -0.02f)
		_facingLeft = true;
	else if (velocity.x > 0.02f)
		_facingLeft = false;
	else
		_textureToDraw = Resources::_textures["idle.png"]; // ĳ���Ͱ� �������� �� ������ animation texture
}

void Player::HandleJump(b2Vec2& velocity)
{
	if (_playerStatus == PlayerStatus::Attacking) // skill ��� ���̸� return
		return;

	// PLAYER_MAX_JUMP_COUNT Ƚ����ŭ ���� �����ϵ��� ���� (���� ������ �������� ����)
	bool currentSpaceState = Keyboard::isKeyPressed(Keyboard::Space); // ������ ���� keyDown, keyReleased �Ǻ��� ���� ���º���

	// KeyDown: Space Ű�� ���Ȱ�, ���� �����ӿ����� ������ �־��ٸ�
	if (currentSpaceState && !_previousSpaceState)
	{
		if (_groundContactCount > 0) // ���� ������ �����ϴ� ����
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
		}

		else if (1 <= _jumpCount && _jumpCount < PLAYER_MAX_JUMP_COUNT) // �̹� ������ �� ���� ���� �õ� ��Ȳ
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
		}
	}

	// KeyReleased: Space Ű�� �������� ��
	if (!currentSpaceState && _previousSpaceState) {
		if (_isJumping) // space Ű�ٿ����� ��ȿ�� ���� ��Ȳ���� key released �Ǿ��� �� (�׳� ������ �������� ���߿��� �����ϴ� �� ����)
		{
			++_jumpCount;
			_isJumping = false;
		}
	}

	_previousSpaceState = currentSpaceState;

	if (_groundContactCount <= 0)
		_textureToDraw = Resources::_textures["jump.png"]; // ĳ���Ͱ� �������� �� (���鿡 ���� ��) ������ animation texture
}

void Player::HandleSkill(float deltaTime, b2Vec2& velocity)
{
	if (_playerStatus == PlayerStatus::Attacking) // �̹� skill ��� ���� ��
	{
		// ��ų �ִϸ��̼� ������Ʈ
		// ��ų ��� �ð��� ���� ��� playerStatus�� Idle�� ����
		SkillManager::getInstance().Update(deltaTime, _currentSkillId);
		
		if (SkillManager::getInstance().isSkillEnd(_currentSkillId))
			_playerStatus = PlayerStatus::Idle;

		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::Q)) // ��������
	{
		velocity = b2Vec2(0.f, 0.f); // ��ų ��� �� ĳ���� �ӵ��� 0���� ����
		_playerStatus = PlayerStatus::Attacking;
		_currentSkillId = (int)SkillList::MELEE_ATTACK;

		// ��ų ���
		SkillManager::getInstance().Begin(_currentSkillId);
	}
		
}
