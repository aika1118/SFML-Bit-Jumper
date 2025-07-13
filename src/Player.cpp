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

	FixtureData* fixtureData = new FixtureData();

	fixtureData->listener = this;
	fixtureData->player = this;
	fixtureData->type = FixtureDataType::Player;

	
	fixtureDef.userData.pointer = (uintptr_t)fixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
	fixtureDef.density = 1.f; // �е�
	fixtureDef.friction = 0.f; // ����

	// ĳ���͸� ���δ� �浹ü
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(PLAYER_SIZE_WIDTH / 2.f, PLAYER_SIZE_HEIGHT / 2.f);
	fixtureDef.shape = &polygonShape;
	body->CreateFixture(&fixtureDef);

	// unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
	unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
	Physics::AddFixtureData(move(fixtureDataPtr));

	// ĳ���� �� �ؿ� ��ġ�� sensor (���� ���� �� ���� ó���� ���� sensor)
	b2FixtureDef sensorFixtureDef;
	b2PolygonShape sensorPolygonShape;

	FixtureData* sensorFixtureData = new FixtureData();

	sensorFixtureData->listener = this;
	sensorFixtureData->player = this;
	sensorFixtureData->type = FixtureDataType::PlayerSensor;

	sensorFixtureDef.userData.pointer = (uintptr_t)sensorFixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
	sensorPolygonShape.SetAsBox(PLAYER_SENSOR_HX, PLAYER_SENSOR_HY, b2Vec2(0.f, PLAYER_SIZE_HEIGHT / 2.f), 0.f);
	sensorFixtureDef.isSensor = true; // sensor : �浹�� ���������� �������� ��ȣ�ۿ��� ���� �ʴ� Ư���� fixture
	sensorFixtureDef.shape = &sensorPolygonShape;

	body->CreateFixture(&sensorFixtureDef);

	// unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
	unique_ptr<FixtureData> sensorFixtureDataPtr(sensorFixtureData);
	Physics::AddFixtureData(move(sensorFixtureDataPtr));

	// ĳ���� �Ӹ� �κп� ��ġ�� sensor
	b2FixtureDef headSensorFixtureDef;
	b2PolygonShape headSensorPolygonShape;

	FixtureData* headFixtureData = new FixtureData();

	headFixtureData->listener = this;
	headFixtureData->player = this;
	headFixtureData->type = FixtureDataType::PlayerHeadSensor;

	headSensorFixtureDef.userData.pointer = (uintptr_t)headFixtureData; // ���� Contact ��Ȳ ��� fixture�� ���� fixtureData ���� ����
	headSensorPolygonShape.SetAsBox(PLAYER_HEAD_SENSOR_HX, PLAYER_HEAD_SENSOR_HY, b2Vec2(0.f, -PLAYER_SIZE_HEIGHT / 2.f), 0.f);
	headSensorFixtureDef.isSensor = true; // sensor : �浹�� ���������� �������� ��ȣ�ۿ��� ���� �ʴ� Ư���� fixture
	headSensorFixtureDef.shape = &headSensorPolygonShape;

	body->CreateFixture(&headSensorFixtureDef);

	// unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
	unique_ptr<FixtureData> headFixtureDataPtr(headFixtureData);
	Physics::AddFixtureData(move(headFixtureDataPtr));

	// �̿� ������� �ʱ�ȭ
	_hp = PLAYER_MAX_HP;
	_isDead = false;
	_playerStatus = PlayerStatus::Idle;
}

void Player::Update(float deltaTime)
{
	_clearTime += deltaTime; // �ʴ��� ����
	_position = Vector2f(body->GetPosition().x, body->GetPosition().y); // Physics::Update()���� world.Step�� ���� ������ �� ���� ��� ������, �̷� ���� body position ��ȭ �߻�
	_angle = body->GetAngle() * (180.f / M_PI); // ���ȿ��� ��(degree) ��ȯ
	b2Vec2 velocity = body->GetLinearVelocity();

	_timeInOneCycle += deltaTime; // �� ���� cycle���� �帣�� time
	if (fabs(_timeInOneCycle - _timeWhenGrounded) * 1000.f >= GAME_OVER_LIMIT_TIME) // ���� �ӹ��� �ð��� GAME_OVER_LIMIT_TIME �ð���ŭ ���� ��� timeOver ó��
	{
		cout << "timeOver !" << endl;
		_isDead = true;
	}
	
	HandleSkill(deltaTime, velocity); // Ű���� �Է¿� ���� skill ó�� (���� �̻��)
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

	renderer.Draw(_textureToDraw, _position, Vector2f(_facingLeft ? -PLAYER_SIZE_WIDTH : PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT), _angle); // ĳ���Ͱ� ���� �ٶ󺸸� size.x�� ���� ����
}


void Player::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	// player�� ���� ������ Fixture�� �´��� ��� ���� �����ϵ��� ó�� (mapTile, boxFragile)
	if (selfData->type == FixtureDataType::PlayerSensor && otherData->canJump == true)
	{
		++_groundContactCount; 

		// jump ���� �ʱ�ȭ
		_jumpCount = 0; 
		_previousSpaceState = false; // ����Ű Ű�ٿ����� ������ ���� ������ �����ϰԵ�

		if (!_isFirstGrounded) // ���� �� ���� ���ʷ� ����� ��
		{
			_isFirstGrounded = true;
			//cout << "First Grounded !" << endl;
			_timeWhenGrounded = _timeInOneCycle; // ���� ���� ������ ��� (���� ������ ����)
		}

		//cout << "Player is on MapTile. Init Progressed !" << endl;

		return;
	}

	// PlayerHeadSensor�� BoxFragile�� ���� ��� box hp�� 1 ���� ��Ŵ (�Ӹ� ��ġ��� Ư�� �ڽ��� �ε��� ��� �ڽ� HP�� 1�� ���ҽ�Ŵ)
	if (selfData->type == FixtureDataType::PlayerHeadSensor && otherData->type == FixtureDataType::Object && otherData->object->_tag == "box_fragile")
	{
		cout << "box head attack!" << endl;
		BoxFragile* box = dynamic_cast<BoxFragile*>(otherData->object); // �����ϰ� �ٿ� ĳ���� (otherData->object�� BoxFragile*�� �ƴҰ�� nullptr-return)
		if (!box) return;
		
		box->TakeDamage(); // �ڽ� ��ġ�� �ϸ� hp 1�� �پ��� �׿� �ش��ϴ� texture�� rendering �� ��

		if (box->getHP() < 0)
		{
			box->destroyBody(); // world���� body�� destroy (��Ȯ���� ���� ��⸮��Ʈ���� �ϴ� �߰� �� Physics���� step ��� �� �ϰ������� destroyBody ����)
			Game::getInstance().DeleteObject(otherData->object); // object�� destroy�Ͽ� ���̻� �ش� object�� ���� update, render���� �ʵ��� ��
		}

		return;
	}

	// player�� coin�� �浹�� ���
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::Object && otherData->object->_tag == "coin")
	{
		Coin* coin = dynamic_cast<Coin*>(otherData->object); // �����ϰ� �ٿ� ĳ���� (otherData->object�� Coin*�� �ƴҰ�� nullptr ��ȯ)
		if (!coin)
			return;
		
		Resources::_sounds["Coin.wav"].play();
		coin->destroyBody(); // world���� body�� destroy (��Ȯ���� ���� ��⸮��Ʈ���� �ϴ� �߰� �� Physics���� step ��� �� �ϰ������� destroyBody ����)
		Game::getInstance().DeleteObject(otherData->object); // object�� destroy�Ͽ� ���̻� �ش� object�� ���� update, render���� �ʵ��� ��

		++_coins;

		// ���� ������ ���� ��� lock ��� ����
		if (_coins == BLOCK_COIN_REQUIRED_TO_OPEN)
		{
			// lock ��� body ����
			for (b2Body* body : Map::getInstance().bodies["lock"])
			{
				if (!body) continue;

				Physics::bodiesToDestroy.insert(body);
			}
			
			// lock ��� texture ����
			for (pair<int, int> pos : Map::getInstance().textures["lock"])
			{
				Map::getInstance()._grid[pos.first][pos.second] = nullptr; // lock ��Ͽ� �ִ� texture�� nullptr�� ��ü
			}
			Map::getInstance().textures["lock"].clear();
		}


		return;
	}

	// player ���ǰ� spike�� �浹�� ��� ��� ó��
	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::Object && otherData->object->_tag == "spike")
	{
		cout << "Spike Attacked!" << endl;
		_hp = max(0, _hp - 1);

		if (_hp <= 0)
			_isDead = true;

		return;
	}

	// player�� ���� �浹�� ���
	if (otherData->type == FixtureDataType::Object && otherData->object->_tag == "enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(otherData->object);
		b2Body* attackBody = self->GetBody();

		if (!enemy)
			return;

		// player ���� fixture (attack box) �� ���� �浹�� ��� �� �ǰ� ó��
		// texture�� �����ð� ���� ���� ������� ��� �׸� (���� objects���� delete ó�� �� ����)

		// selfData�� ��ų�̸� ���� ���� Ÿ������ ���� ���
		if (selfData->type == FixtureDataType::Attack && !selfData->isSkillAttacked)
		{
			--enemy->_hp;
			selfData->isSkillAttacked = true; // ��ų�� ���� Ÿ���� ��� ��ų Ÿ�� ó��

			enemy->_isEnemyAttacked = true;

			if (enemy->_hp <= 0)
				enemy->SetDead(); // Physics::bodiesToDestroy.push_back()�� ���� ����
		}

		// Player�� �� �� ������ ���� �浹�� ��� �� óġ ó�� (���� ������ ���� �� ����)
		if (selfData->type == FixtureDataType::PlayerSensor) 
		{
			cout << "Enemy Killed!" << endl;
			enemy->SetDead();
			return;
		}

		// enemy�� ����ְ� player�� ���� �Ϲ������� �浹�� ��� �ǰ�ó��
		if (enemy->IsDead() == false)
		{
			cout << "Enemy Attacked!" << endl;
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

		Resources::_sounds["SaveTile.wav"].play();

		return;
	}

	// player�� exit Ÿ�ϰ� �浹�� ��� ���� ���� ó��
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::ExitTile)
	{
		cout << "Stage : " << Game::getInstance().getStageSelected() + 1 << " Cleared" << endl;
		cout << "Current Percentage : " << _judgementPercentage << endl;

		// ���� ó��
		Resources::_musics["Music_Game.wav"].stop();
		Resources::_sounds["Clear.wav"].play();

		// ���� ��� ���

		// [player]�� ��ϵǴ� ������ ���� ��� ������ ? Game�� player id ���� ����ǰ� �ֱ� ����
		_currentClearStage = Game::getInstance().getStageSelected();
		_stageScores[Game::getInstance().getStageSelected()] = _judgementPercentage;

		// [Game]�� ���� Ŭ���� �������� �� ������ ����
		Game::getInstance().setPlayerCurrentClearStage(Game::getInstance().getUid(), _currentClearStage);
		Game::getInstance().setPlayerStageScore(Game::getInstance().getUid(), _currentClearStage, _judgementPercentage);

		if (Game::getInstance().isServerConnected())
		{
			// ������ id, ��������, ����, �ð� ������ ��Ŷ���� ����
			string data = to_string(Game::getInstance().getUid()) + ","
				+ to_string(_currentClearStage) + ","
				+ to_string(_judgementPercentage) + ","
				+ to_string(_clearTime);

			// DB�� ���� ����� ���� (DB ���� �� �߰� �۾��� ����, �޸𸮿��� �̹� ����� ����Ǿ� �ִ� ����)
			Game::getInstance().getClient()->send_packet_async(PACKET_WRITE, data,
				[](const string& response) { // �ݹ� ����
					return;
				}
			);
		}
		

		// state�� ���� ���� �޴��� ����
		Game::getInstance().setMenuState(MenuIndex::CLEAR_MENU);
		

		return;
	}

	// player�� ������ ������� �����ð� ������ ���
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::Object && otherData->object->_tag == "mushroom")
	{	
		Mushroom* mushroom = dynamic_cast<Mushroom*>(otherData->object); // �����ϰ� �ٿ� ĳ���� (otherData->object�� Mushroom*�� �ƴҰ�� nullptr ��ȯ)
		if (!mushroom) return;
		if (bIsJumpImproved == true) return; // �̹� ���� ��� ȿ���� �޴� ���̶�� return ó��

		// ������ ���� object�� �ı����� �ʰ� Mushroom Ŭ�������� �������� ���� �ʵ��� ó����
		bIsJumpImproved = true;
		Resources::_sounds["Mushroom.wav"].play();
		

		cout << "jump improve on" << endl;
	}

}

void Player::OnEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	if (selfData->type == FixtureDataType::PlayerSensor && otherData->canJump == true) // player�� ���� fixture�� �־����� ��� (mapTile, boxFragile)
	{
		_groundContactCount = _groundContactCount > 0 ? --_groundContactCount : _groundContactCount;
		return;
	}
}

float Player::getJudgementPercentage()
{
	return _judgementPercentage;
}

string Player::getJudgementCurrent()
{
	return _judgementCurrent;
}

int Player::getCurrentClearStage()
{
	return _currentClearStage;
}

int Player::getCoin()
{
	return _coins;
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
	
	// ����Ű�� �������� _timeWhenJumpKeydown ���� �ʱ�ȭ ������ �� ����Ű ���� ������ ��� (for ����)
	if (_timeWhenJumpKeydown == 0.f && currentSpaceState) 
		_timeWhenJumpKeydown = _timeInOneCycle;

	// KeyDown: Space Ű�� ���Ȱ�, ���� �����ӿ����� ������ �־��ٸ�
	if (currentSpaceState && !_previousSpaceState)
	{
		//cout << "Space Pressed !" << endl;
		//cout << "_groundContactCount : " << _groundContactCount << endl;

		if (_groundContactCount > 0) // ���� ������ �����ϴ� ����
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			if (bIsJumpImproved == true) velocity.y *= sqrt(PLAYER_JUMP_IMPROVEMENT_SIZE); // ���� ���ȿ�� ���� �� ���̰� PLAYER_JUMP_IMPROVEMENT_SIZE �����ŭ ����
			_isJumping = true;
			_isFirstGrounded = false;
			//cout << "Jump Success (Init) !" << endl;

			// ���� ������ ���� ����Ű�� ���� �ð� (_timeWhenJumpKeydown)���� ������ ���� ���� ���� �ð� (_timeWhenGrounded) ���̷� ���� ���
			float judgementTime = fabs(_timeWhenJumpKeydown - _timeWhenGrounded) * 1000.f;
			//cout << judgementTime << "ms" << endl;
			if (judgementTime <= JUDGEMENT_PERFECT || _judgementTotalCnt == 0) // ó�� �������� ���� ������ Perfect ó��
			{ 
				Resources::_sounds["Jump_Perfect.wav"].play();
				_judgementPerfectCnt += 1;
				_judgementCurrent = "Perfect";
				cout << "Perfect" << endl;
			}
			else if (judgementTime <= JUDGEMENT_GREAT)
			{
				Resources::_sounds["Jump_Great.wav"].play();
				_judgementGreatCnt += 1;
				_judgementCurrent = "Great";
				cout << "Great" << endl;
			}
			else
			{
				Resources::_sounds["Jump_Miss.wav"].play();
				_judgementMissCnt += 1;
				_judgementCurrent = "Miss";
				cout << "Miss" << endl;
			}

			_judgementTotalCnt += 1;

			_judgementPercentage = 100.f * (_judgementPerfectCnt * 1.f + _judgementGreatCnt * 0.5f + _judgementMissCnt * 0.f) / _judgementTotalCnt;
		}

		else if (1 <= _jumpCount && _jumpCount < PLAYER_MAX_JUMP_COUNT) // �̹� ������ �� ���� ���� �õ� ��Ȳ
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
			_isFirstGrounded = false;
			//cout << "Jump Success (Continuous) !" << endl;
		}
	}

	// KeyReleased: Space Ű�� �������� ��
	else if (!currentSpaceState && _previousSpaceState) {
		if (_isJumping) // space Ű�ٿ����� ��ȿ�� ���� ��Ȳ���� key released �Ǿ��� �� (�׳� ������ �������� ���߿��� �����ϴ� �� ����)
		{
			++_jumpCount;
			_isJumping = false;
			//cout << "Jump Released !" << endl;

			// ���� ó�� �� key�� released �Ǿ��� �� ������ �����ϴ� �ð������� �ʱ�ȭ
			_timeInOneCycle = 0.f;
			_timeWhenJumpKeydown = 0.f;
			_timeWhenGrounded = 0.f;
		}
	}

	else return;

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

	//
	// _currentSkillId ��ų ��Ÿ�� �� ��ų ��� �Ұ� �ڵ��߰� �ʿ�
	// 

	if (Keyboard::isKeyPressed(Keyboard::Q)) // ��������
	{
		velocity = b2Vec2(0.f, 0.f); // ��ų ��� �� ĳ���� �ӵ��� 0���� ����
		_playerStatus = PlayerStatus::Attacking;
		_currentSkillId = (int)SkillList::MELEE_ATTACK;

		// ��ų ���
		SkillManager::getInstance().Begin(_currentSkillId);
	}
		
}
