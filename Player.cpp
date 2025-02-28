#include "Player.h"
#include "Game.h" // 순환참조 회피


void Player::Begin()
{
	// Animation Frame 정의
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
	polygonShape.SetAsBox(PLAYER_SIZE_WIDTH / 2.f, PLAYER_SIZE_HEIGHT / 2.f);
	fixtureDef.shape = &polygonShape;
	body->CreateFixture(&fixtureDef);

	// 캐릭터 발 밑에 설치할 sensor (땅에 닿을 때 점프 처리를 위한 sensor)

	b2FixtureDef sensorFixtureDef;
	b2PolygonShape sensorPolygonShape;

	_sensorFixtureData.listener = this;
	_sensorFixtureData.player = this;
	_sensorFixtureData.type = FixtureDataType::PlayerSensor;

	sensorFixtureDef.userData.pointer = (uintptr_t)&_sensorFixtureData; // 차후 Contact 상황 등에서 fixture를 통해 fixtureData 접근 가능
	sensorPolygonShape.SetAsBox(PLAYER_SENSOR_HX, PLAYER_SENSOR_HY, b2Vec2(0.f, PLAYER_SIZE_HEIGHT / 2.f), 0.f);
	sensorFixtureDef.isSensor = true; // sensor : 충돌을 감지하지만 물리적인 상호작용은 하지 않는 특수한 fixture
	sensorFixtureDef.shape = &sensorPolygonShape;

	body->CreateFixture(&sensorFixtureDef);

	// 캐릭터 머리 부분에 설치할 sensor

	b2FixtureDef headSensorFixtureDef;
	b2PolygonShape headSensorPolygonShape;

	_headSensorFixtureData.listener = this;
	_headSensorFixtureData.player = this;
	_headSensorFixtureData.type = FixtureDataType::PlayerHeadSensor;

	headSensorFixtureDef.userData.pointer = (uintptr_t)&_headSensorFixtureData; // 차후 Contact 상황 등에서 fixture를 통해 fixtureData 접근 가능
	headSensorPolygonShape.SetAsBox(PLAYER_HEAD_SENSOR_HX, PLAYER_HEAD_SENSOR_HY, b2Vec2(0.f, -PLAYER_SIZE_HEIGHT / 2.f), 0.f);
	headSensorFixtureDef.isSensor = true; // sensor : 충돌을 감지하지만 물리적인 상호작용은 하지 않는 특수한 fixture
	headSensorFixtureDef.shape = &headSensorPolygonShape;

	body->CreateFixture(&headSensorFixtureDef);

	// 이외 멤버함수 초기화
	_hp = PLAYER_MAX_HP;
	_isDead = false;
	_playerStatus = PlayerStatus::Idle;
}

void Player::Update(float deltaTime)
{
	_clearTime += deltaTime; // 초단위 저장
	_position = Vector2f(body->GetPosition().x, body->GetPosition().y); // Physics::Update()에서 world.Step에 의해 프레임 당 물리 계산 적용중, 이로 인해 body position 변화 발생
	_angle = body->GetAngle() * (180.f / M_PI); // 라디안에서 도(degree) 변환
	b2Vec2 velocity = body->GetLinearVelocity();

	_timeInOneCycle += deltaTime; // 한 점프 cycle마다 흐르는 time
	if (fabs(_timeInOneCycle - _timeWhenGrounded) * 1000.f >= GAME_OVER_LIMIT_TIME) // 땅에 머무는 시간이 2초가 지난 경우 timeOver 처리
	{
		cout << "timeOver !" << endl;
		_isDead = true;
	}
	
	HandleSkill(deltaTime, velocity); // 키보드 입력에 따라 skill 처리
	HandleMove(deltaTime, velocity); // 키보드 입력에 따라 move 처리
	HandleJump(velocity); // 키보드 입력에 따라 jump 처리
	body->SetLinearVelocity(velocity);
}

void Player::Draw(Renderer& renderer)
{
	if (_playerStatus == PlayerStatus::Attacking) // 스킬 사용 중일 때는 스킬 렌더링
	{
		SkillManager::getInstance().Render(renderer, _currentSkillId);
		return;
	}

	renderer.Draw(_textureToDraw, _position, Vector2f(_facingLeft ? -PLAYER_SIZE_WIDTH : PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT), _angle); // 캐릭터가 왼쪽 바라보면 size.x에 음수 적용
}


void Player::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	// player가 점프 가능한 Fixture와 맞닿은 경우 점프 가능하도록 처리 (mapTile, boxFragile)
	if (selfData->type == FixtureDataType::PlayerSensor && otherData->canJump == true)
	{
		++_groundContactCount; 

		// jump 상태 초기화
		_jumpCount = 0; 
		_previousSpaceState = false; // 점프키 키다운으로 땅에서 연속 점프가 가능하게됨

		if (!_isFirstGrounded) // 점프 후 땅이 최초로 닿았을 때
		{
			_isFirstGrounded = true;
			//cout << "First Grounded !" << endl;
			_timeWhenGrounded = _timeInOneCycle; // 땅이 닿은 시점을 기록 (for 판정)
		}

		//cout << "Player is on MapTile. Init Progressed !" << endl;

		return;
	}

	// PlayerHeadSensor가 BoxFragile과 닫은 경우 box hp를 1 감소 시킴 (머리 박치기로 특수 박스와 부딪힌 경우 박스 HP를 1씩 감소시킴)
	if (selfData->type == FixtureDataType::PlayerHeadSensor && otherData->type == FixtureDataType::Object && otherData->object->_tag == "box_fragile")
	{
		cout << "box head attack!" << endl;
		BoxFragile* box = dynamic_cast<BoxFragile*>(otherData->object); // 안전하게 다운 캐스팅 (otherData->object가 BoxFragile*이 아닐경우 nullptr-return)
		if (!box) return;
		
		box->TakeDamage(); // 박스 박치기 하면 hp 1씩 줄어들며 그에 해당하는 texture로 rendering 될 것

		if (box->getHP() < 0)
		{
			box->destroyBody(); // world에서 body를 destroy (정확히는 삭제 대기리스트에만 일단 추가 후 Physics에서 step 계산 후 일괄적으로 destroyBody 진행)
			Game::getInstance().DeleteObject(otherData->object); // object를 destroy하여 더이상 해당 object에 대해 update, render되지 않도록 함
		}

		return;
	}

	// player가 coin과 충돌한 경우
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::Object && otherData->object->_tag == "coin")
	{
		Coin* coin = dynamic_cast<Coin*>(otherData->object); // 안전하게 다운 캐스팅 (otherData->object가 Coin*이 아닐경우 nullptr 반환)
		if (!coin)
			return;
		
		coin->destroyBody(); // world에서 body를 destroy (정확히는 삭제 대기리스트에만 일단 추가 후 Physics에서 step 계산 후 일괄적으로 destroyBody 진행)
		Game::getInstance().DeleteObject(otherData->object); // object를 destroy하여 더이상 해당 object에 대해 update, render되지 않도록 함

		++_coins;

		//cout << "coins = " << _coins << endl;

		return;
	}

	// player 발판과 spike와 충돌한 경우 사망 처리
	if (selfData->type == FixtureDataType::PlayerSensor && otherData->type == FixtureDataType::Object && otherData->object->_tag == "spike")
	{
		cout << "Spike Attacked!" << endl;
		_hp = max(0, _hp - 1);

		if (_hp <= 0)
			_isDead = true;

		return;
	}

	// player가 적과 충돌한 경우
	if (otherData->type == FixtureDataType::Object && otherData->object->_tag == "enemy")
	{
		Enemy* enemy = dynamic_cast<Enemy*>(otherData->object);
		b2Body* attackBody = self->GetBody();

		if (!enemy)
			return;

		// player 공격 fixture (attack box) 가 적과 충돌한 경우 적 피격 처리
		// texture는 일정시간 동안 죽은 모습으로 계속 그림 (이후 objects에서 delete 처리 될 예정)

		// selfData가 스킬이며 아직 적을 타격하지 않은 경우
		if (selfData->type == FixtureDataType::Attack && !selfData->isSkillAttacked)
		{
			--enemy->_hp;
			selfData->isSkillAttacked = true; // 스킬로 적을 타격한 경우 스킬 타격 처리

			enemy->_isEnemyAttacked = true;

			if (enemy->_hp <= 0)
				enemy->destroyBody(); // Physics::bodiesToDestroy.push_back()랑 같은 역할
		}

		// Player가 발 밑 센서로 적과 충돌한 경우 적 처치 처리 (적을 밟으면 죽일 수 있음)
		if (selfData->type == FixtureDataType::PlayerSensor) 
		{
			cout << "Enemy Killed!" << endl;
			enemy->destroyBody();
			return;
		}

		// enemy가 살아있고 player가 적과 일반적으로 충돌한 경우 피격처리
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

	// player가 save 타일과 충돌한 경우
	// 최근에 충돌한 save 타일에 리스폰 위치 지정하는 작업
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::SaveTile)
	{
		int preSaveX = Game::getInstance()._savePositionX;
		int preSaveY = Game::getInstance()._savePositionY;
		int curSaveX = otherData->mapX;
		int curSaveY = otherData->mapY;
		
		// 이전에 세이브한 장소와 동일한 경우 바로 return 처리
		if (preSaveX == curSaveX && preSaveY == curSaveY)
			return;

		// 현재 세이브 장소의 texture를 save_used.png로 변경
		Map::getInstance()._grid[curSaveX][curSaveY] = &Resources::_textures["save_used.png"];

		// 이전 세이브 기록이 있는 경우 이전 세이브 블록을 원본으로 돌려놓음
		if (preSaveX || preSaveY)
			Map::getInstance()._grid[preSaveX][preSaveY] = &Resources::_textures["save.png"];

		// 최근 세이브 장소를 Game 클래스에 기록
		Game::getInstance()._savePositionX = curSaveX;
		Game::getInstance()._savePositionY = curSaveY;

		return;
	}

	// player가 exit 타일과 충돌한 경우 게임 종료 처리
	if (selfData->type == FixtureDataType::Player && otherData->type == FixtureDataType::ExitTile)
	{
		cout << "Stage : " << Game::getInstance().getStageSelected() + 1 << " Cleared" << endl;
		cout << "Current Percentage : " << _judgementPercentage << endl;

		// 게임 결과 기록

		// [player]에 기록되는 정보는 구지 없어도 될지도 ? Game에 player id 별로 저장되고 있기 떄문
		_currentClearStage = Game::getInstance().getStageSelected();
		_stageScores[Game::getInstance().getStageSelected()] = _judgementPercentage;

		// [Game]에 현재 클리어 스테이지 및 점수를 저장
		Game::getInstance().setPlayerCurrentClearStage(Game::getInstance().getUid(), _currentClearStage);
		Game::getInstance().setPlayerStageScore(Game::getInstance().getUid(), _currentClearStage, _judgementPercentage);

		// 서버에 id, 스테이지, 점수, 시간 정보를 패킷으로 전달
		string data = to_string(Game::getInstance().getUid()) + ","
					+ to_string(_currentClearStage) + "," 
					+ to_string(_judgementPercentage) + "," 
					+ to_string(_clearTime);

		if (Util::isServerConnected())
			Game::getInstance().getClient()->send_packet_async(PACKET_WRITE,data);	

		// state를 게임 종료 메뉴로 설정
		Game::getInstance().setMenuState(MenuIndex::CLEAR_MENU);

		return;
	}
}

void Player::OnEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer; // self
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer; // other

	if (!selfData) return;
	if (!otherData)	return;

	if (selfData->type == FixtureDataType::PlayerSensor && otherData->canJump == true) // player가 점프 fixture와 멀어지는 경우 (mapTile, boxFragile)
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

void Player::HandleMove(float deltaTime, b2Vec2& velocity)
{
	if (_playerStatus == PlayerStatus::Attacking) // skill 사용 중이면 return
		return;

	float move = PLAYER_MOVE_VELOCITY;

	if (Keyboard::isKeyPressed(Keyboard::LShift)) // shift 키다운 시 이동속도 2배
		move *= 2;

	
	velocity.x = 0.f;

	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x = +move;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x = -move;

	// 현재 time에 따라 draw할 texture 얻기
	_runAnimation.Update(deltaTime);
	_textureToDraw = _runAnimation.GetTexture();

	if (velocity.x < -0.02f)
		_facingLeft = true;
	else if (velocity.x > 0.02f)
		_facingLeft = false;
	else
		_textureToDraw = Resources::_textures["idle.png"]; // 캐릭터가 정지중일 때 적용할 animation texture
}

void Player::HandleJump(b2Vec2& velocity)
{
	if (_playerStatus == PlayerStatus::Attacking) // skill 사용 중이면 return
		return;

	// PLAYER_MAX_JUMP_COUNT 횟수만큼 점프 가능하도록 구현 (최초 점프는 땅에서만 가능)
	bool currentSpaceState = Keyboard::isKeyPressed(Keyboard::Space); // 프레임 별로 keyDown, keyReleased 판별을 위한 상태변수
	
	// 점프키를 눌렀으며 _timeWhenJumpKeydown 값이 초기화 상태일 때 점프키 누른 시점을 기록 (for 판정)
	if (_timeWhenJumpKeydown == 0.f && currentSpaceState) 
		_timeWhenJumpKeydown = _timeInOneCycle;

	// KeyDown: Space 키가 눌렸고, 이전 프레임에서는 떼어져 있었다면
	if (currentSpaceState && !_previousSpaceState)
	{
		//cout << "Space Pressed !" << endl;
		//cout << "_groundContactCount : " << _groundContactCount << endl;

		if (_groundContactCount > 0) // 최초 땅에서 점프하는 순간
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
			_isFirstGrounded = false;
			//cout << "Jump Success (Init) !" << endl;

			// 현재 점프를 위해 점프키를 누른 시간 (_timeWhenJumpKeydown)에서 점프를 위해 땅에 닿은 시간 (_timeWhenGrounded) 차이로 판정 계산
			float judgementTime = fabs(_timeWhenJumpKeydown - _timeWhenGrounded) * 1000.f;
			//cout << judgementTime << "ms" << endl;
			if (judgementTime <= JUDGEMENT_PERFECT || _judgementTotalCnt == 0) // 처음 점프했을 때는 무조건 Perfect 처리
			{ 
				_judgementPerfectCnt += 1;
				_judgementCurrent = "Perfect";
				cout << "Perfect" << endl;
			}
			else if (judgementTime <= JUDGEMENT_GREAT)
			{
				_judgementGreatCnt += 1;
				_judgementCurrent = "Great";
				cout << "Great" << endl;
			}
			else
			{
				_judgementMissCnt += 1;
				_judgementCurrent = "Miss";
				cout << "Miss" << endl;
			}

			_judgementTotalCnt += 1;

			_judgementPercentage = 100.f * (_judgementPerfectCnt * 1.f + _judgementGreatCnt * 0.5f + _judgementMissCnt * 0.f) / _judgementTotalCnt;
		}

		else if (1 <= _jumpCount && _jumpCount < PLAYER_MAX_JUMP_COUNT) // 이미 점프한 후 연속 점프 시도 상황
		{
			velocity.y = PLAYER_JUMP_VELOCITY;
			_isJumping = true;
			_isFirstGrounded = false;
			//cout << "Jump Success (Continuous) !" << endl;
		}
	}

	// KeyReleased: Space 키가 떼어졌을 때
	else if (!currentSpaceState && _previousSpaceState) {
		if (_isJumping) // space 키다운으로 유효한 점프 상황에서 key released 되었을 때 (그냥 땅에서 떨어지며 공중에서 점프하는 것 방지)
		{
			++_jumpCount;
			_isJumping = false;
			//cout << "Jump Released !" << endl;

			// 점프 처리 후 key가 released 되었을 때 판정을 관리하는 시간변수들 초기화
			_timeInOneCycle = 0.f;
			_timeWhenJumpKeydown = 0.f;
			_timeWhenGrounded = 0.f;
		}
	}

	else return;

	_previousSpaceState = currentSpaceState;

	if (_groundContactCount <= 0)
		_textureToDraw = Resources::_textures["jump.png"]; // 캐릭터가 점프중일 때 (지면에 없을 떄) 적용할 animation texture
}

void Player::HandleSkill(float deltaTime, b2Vec2& velocity)
{
	if (_playerStatus == PlayerStatus::Attacking) // 이미 skill 사용 중일 떄
	{
		// 스킬 애니메이션 업데이트
		// 스킬 사용 시간이 끝난 경우 playerStatus를 Idle로 변경
		SkillManager::getInstance().Update(deltaTime, _currentSkillId);
		
		if (SkillManager::getInstance().isSkillEnd(_currentSkillId))
			_playerStatus = PlayerStatus::Idle;

		return;
	}

	//
	// _currentSkillId 스킬 쿨타임 중 스킬 사용 불가 코드추가 필요
	// 

	if (Keyboard::isKeyPressed(Keyboard::Q)) // 근접공격
	{
		velocity = b2Vec2(0.f, 0.f); // 스킬 사용 전 캐릭터 속도를 0으로 만듦
		_playerStatus = PlayerStatus::Attacking;
		_currentSkillId = (int)SkillList::MELEE_ATTACK;

		// 스킬 사용
		SkillManager::getInstance().Begin(_currentSkillId);
	}
		
}
