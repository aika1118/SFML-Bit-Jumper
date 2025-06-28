#include "Game.h"


Game::Game() // 참조자 멤버변수 초기화
	:gameMap(Map::getInstance()), camera(Camera::getInstance())
{
	if (Util::checkServerConnection() == false) // 오프라인 모드일 경우
	{
		cout << "Offline Mode !" << endl;
		_isServerConnected = false;
	}

	else
	{
		try
		{
			client = new Client(io_context, SERVER_IP, SERVER_PORT); // 클라이언트 생성
			cout << "Client Connected !" << endl;
		}
		catch (const std::exception& e)
		{
			cerr << "Exception: " << e.what() << "\n"; // 예외처리
		}
	}	
	
	Physics::Init();
	_enemyPool = new EnemyPool();
}

int Game::getUid() const
{
	return _uid;
}

void Game::setUid(int uid)
{
	_uid = uid;
}

Game& Game::getInstance()
{
	static Game instance; // 인스턴스는 최초 호출 시에만 생성됨
	return instance;
}


void Game::Begin(RenderWindow& window)
{
	for (const auto& file : filesystem::directory_iterator("./resources/textures/")) // 해당 경로에 있는 모든 texture 불러오기
	{
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg"))
			Resources::_textures[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map에 texture 정보 저장
	}

	for (const auto& file : filesystem::directory_iterator("./resources/sounds/use/")) // sound 불러오기
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
		{
			// unordered_map에 sound 정보 저장
			if (!Resources::soundBuffers[file.path().filename().string()].loadFromFile(file.path().string()))
			{
				std::cerr << "Failed to load sound: " << file.path().string() << std::endl;
				continue;
			}

			Resources::_sounds[file.path().filename().string()].setBuffer(Resources::soundBuffers[file.path().filename().string()]);
			Resources::_sounds[file.path().filename().string()].setVolume(VOLUME_SOUND);
		}
	}

	for (const auto& file : filesystem::directory_iterator("./resources/musics/use/")) // sound 불러오기
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
		{
			// unordered_map에 music 정보 저장
			if (!Resources::_musics[file.path().filename().string()].openFromFile(file.path().string()))
			{
				std::cerr << "Failed to load music: " << file.path().string() << std::endl;
				continue;
			}
			Resources::_musics[file.path().filename().string()].setLoop(true);
			Resources::_musics[file.path().filename().string()].setVolume(VOLUME_MUSIC);
		}
	}

	//_mapImage.loadFromFile(MAP_STAGE_1);
	//_mapBound = FloatRect(0.f, 0.f, (float)_mapImage.getSize().x, (float)_mapImage.getSize().y); // 현재 mapBound 계산 (view가 맵 경계 벗어나지 않도록 하는 작업)

	if (!font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
	{
		cerr << "font load error!" << endl;
		return;
	}

	// Text 초기화 하는거 함수화 필요
		
	playerJudgementPercentageText.setFont(font);
	playerJudgementPercentageText.setFillColor(Color::White);
	playerJudgementPercentageText.setOutlineColor(Color::Black);
	playerJudgementPercentageText.setOutlineThickness(1.f);
	playerJudgementPercentageText.setScale(UI_CHARACTER_SCALE, UI_CHARACTER_SCALE);

	playerJudgementText.setFont(font);
	playerJudgementText.setFillColor(Color::White);
	playerJudgementText.setOutlineColor(Color::Black);
	playerJudgementText.setOutlineThickness(1.f);
	playerJudgementText.setScale(UI_CHARACTER_SCALE, UI_CHARACTER_SCALE);

	gameOverText.setFont(font);
	gameOverText.setFillColor(Color::White);
	gameOverText.setOutlineColor(Color::Black);
	gameOverText.setOutlineThickness(1.f);
	gameOverText.setScale(UI_CHARACTER_SCALE, UI_CHARACTER_SCALE);

	continueText.setFont(font);
	continueText.setFillColor(Color::White);
	continueText.setOutlineColor(Color::Black);
	continueText.setOutlineThickness(1.f);
	continueText.setScale(UI_CHARACTER_SCALE, UI_CHARACTER_SCALE);
	continueText.setString("Continue");
	continueText.setOrigin(continueText.getLocalBounds().left + continueText.getLocalBounds().width / 2.0f, continueText.getLocalBounds().top + continueText.getLocalBounds().height / 2.0f);

	restartText.setFont(font);
	restartText.setFillColor(Color::White);
	restartText.setOutlineColor(Color::Black);
	restartText.setOutlineThickness(1.f);
	restartText.setScale(UI_CHARACTER_SCALE, UI_CHARACTER_SCALE);
	restartText.setString("Restart");
	restartText.setOrigin(restartText.getLocalBounds().left + restartText.getLocalBounds().width / 2.0f, restartText.getLocalBounds().top + restartText.getLocalBounds().height / 2.0f);

	exitText.setFont(font);
	exitText.setFillColor(Color::White);
	exitText.setOutlineColor(Color::Black);
	exitText.setOutlineThickness(1.f);
	exitText.setScale(UI_CHARACTER_SCALE, UI_CHARACTER_SCALE);
	exitText.setString("Exit");
	exitText.setOrigin(exitText.getLocalBounds().left + exitText.getLocalBounds().width / 2.0f, exitText.getLocalBounds().top + exitText.getLocalBounds().height / 2.0f);

	playerCoinText.setFont(font);
	playerCoinText.setFillColor(Color::White);
	playerCoinText.setOutlineColor(Color::Black);
	playerCoinText.setOutlineThickness(1.f);
	playerCoinText.setScale(UI_CHARACTER_SCALE/1.2f, UI_CHARACTER_SCALE/1.2f);

	backgroundWhenPaused.setSize(Vector2f(1.f, 1.f));
	backgroundWhenPaused.setFillColor(Color(0, 0, 0, 150)); // 알파값만 조정
	backgroundWhenPaused.setOrigin(0.5f, 0.5f); // origin을 중심으로 맞추기

	MenuManager::getInstance().init(window);

	// 서버연결 안되어있으면 진행 스테이지 상황을 기본값으로 초기화 (_uid는 이미 SETTING_UID_NOT_INITED 값으로 초기화되어있음)
	if (_isServerConnected == false) setPlayerCurrentClearStage(getUid(), PLAYER_DEFAULT_STAGE);

	InitSkill();
}

void Game::InitSkill()
{
	// melee skill 초기화
	int meleeStart = (int)SkillList::MELEE_START;
	int meleeEnd = (int)SkillList::MELEE_END;

	for (int skillId = meleeStart + 1; skillId < meleeEnd; ++skillId)
	{
		Skill* skill = new SkillMeleeAttack(skillId); // skillId에 따라 스킬 고유한 객체 생성
		SkillManager::getInstance().AddSkill(skillId, skill); // skillId에 매핑하여 skill을 unordered_map에 추가
	}
}

Vector2f Game::getPlayerPosition()
{
	return player._position;
}

float Game::getPlayerAngle()
{
	return player._angle;
}

bool Game::getPlayerFacingLeft()
{
	return player._facingLeft;
}

Player& Game::getPlayer()
{
	return player;
}

int& Game::getMenuState()
{
	return _menuState;
}

int Game::getStageSelected()
{
	return _stageSelected;
}

void Game::setStageSelected(int stage)
{
	_stageSelected = stage;
}

void Game::setMapImage(int stage)
{
	_mapImage.loadFromFile(Map::getInstance().getMapImages(stage)); // load 실패할 때 예외처리 필요
	_mapBound = FloatRect(0.f, 0.f, (float)_mapImage.getSize().x, (float)_mapImage.getSize().y); // 현재 mapBound 계산 (view가 맵 경계 벗어나지 않도록 하는 작업)
	Restart();
}

void Game::setMenuState(int menuState)
{
	_menuState = menuState;
}

int Game::getPlayerCurrentClearStage(int id)
{
	lock_guard<mutex> lock(playerCurrentClearStages_mutex_);
	return _playerCurrentClearStages[id];
}

void Game::setPlayerCurrentClearStage(int id, int stage)
{
	lock_guard<mutex> lock(playerCurrentClearStages_mutex_);
	_playerCurrentClearStages[id] = stage;
}

float Game::getPlayerStageScore(int id, int stage)
{
	return _playerStageScores[id][stage];
}

void Game::setPlayerStageScore(int id, int stage, float score)
{
	_playerStageScores[id][stage] = score;
}

bool Game::isServerConnected()
{
	return _isServerConnected;
}


Client* Game::getClient()
{
	return client;
}

io_context& Game::getIoContext()
{
	return io_context;
}

string Game::getUsername()
{
	return _username;
}

EnemyPool* Game::GetEnemyPool()
{
	return _enemyPool;
}

void Game::setWindowClosed(bool flag)
{
	_isWindowClosed = flag;
}

bool Game::getWindowClosed()
{
	return _isWindowClosed;
}

void Game::Restart() // Begin할 때의 Restart()가 뭔가 중복되는 것 같아서 코드 정리 필요 (world도 Game 생성자에서 Init하고 Restart()에서 또 Init 되는듯 하여 문제 없을지 확인)
{
	cout << "Restart() Called !" << endl;
	InitObject();  // 현재 world의 object 관련 정보를 담고있는 vector 초기화 (restart 후 이전 object들은 렌더링 되지 않도록)
	Physics::Init(); // 기존 world 초기화
	_enemyPool->Reset(); // enemyPool 초기화

	// 세이브 포인트 초기화
	_savePositionX = PLAYER_NO_SAVE_POSITION;
	_savePositionY = PLAYER_NO_SAVE_POSITION;

	player = Player(); // 임시 객체를 생성하고, 복사 대입 연산자로 새로 초기화
	gameMap.CreateFromImage(_mapImage, _objects); // map 따라 player 위치 및 object 생성

	player._position = _playerPosition;
	player.Begin();

	for (Object* object : _objects)
		object->Begin();

	

	// 메뉴 bgm 정지 후 인게임 bgm 출력
	if (Resources::_musics["Music_Menu.wav"].getStatus() == Music::Playing) Resources::_musics["Music_Menu.wav"].stop();
	Resources::_musics["Music_Game.wav"].play();

	_isGameResourceCleaned = false; // 게임 종료 후 리소스 정리 여부를 체크하는 flag
}

void Game::Update(float deltaTime, RenderWindow& window)
{
	if (getWindowClosed() == true) return; // 게임 window가 닫힌경우 더이상 game 업데이트 하지 않음

	if (_paused) return; // pause 상황이면 game 업데이트 하지 않음

	// ESC 키를 눌러 pause 한 경우
	if (!_paused && Keyboard::isKeyPressed(Keyboard::Escape))
	{
		_paused = !_paused;
		cout << "Paused !" << endl;
	}

	if (_isServerConnected && !_isUidInited) // uid가 초기화 될때까지 해당 if 조건에 계속 걸리게됨
	{
		// 현재 닉네임 생성 메뉴에 있다면 return
		if (_menuState == MenuIndex::MAKE_USERNAME_MENU) return;

		_uid = Util::getUID();

		// 아직 세팅파일에 uid가 없는 경우
		if (_uid == SETTING_UID_NOT_INITED) setMenuState(MenuIndex::MAKE_USERNAME_MENU); // 닉네임 생성 메뉴로 이동

		else // 정상적인 uid를 얻은 경우
		{
			_username = Util::getUserName(_uid); // uid를 얻은 경우 username도 get 하기
			_menuState = MenuIndex::MAIN_MENU; // 메인 메뉴로 이동

			// uid에 해당하는 최신 clear stage를 init 값으로 초기화 (실제 db값 받기전에 로딩중을 알리는 처리를 하기 위함 in StageMenu.cpp)
			setPlayerCurrentClearStage(_uid, SETTING_PLAYER_CURRENT_CLEAR_STAGE_NOT_INITED);
			
			// uid에 해당하는 최신 clear stage 정보 받아오기
			if (Game::getInstance().isServerConnected())
				client->send_packet_async(PACKET_READ_MAX_CLEAR_STAGE, to_string(_uid),
					[](const string& response) { // 콜백 정의
						cout << "PACKET_READ_MAX_CLEAR_STAGE callback!" << endl;
						// uid에 해당하는 가장 높이 클리어했던 stage 저장
						Game::getInstance().setPlayerCurrentClearStage(Game::getInstance().getUid(), stoi(response));
						return;
					}
				);
			else // Update() if 조건에서 서버연결을 확인했지만 혹시 몰라 예외처리
				setPlayerCurrentClearStage(_uid, PLAYER_DEFAULT_STAGE);

			_isUidInited = true;
		}
	}

	MenuManager::getInstance().setCurrentMenu(_menuState);

	if (_menuState == MenuIndex::EXIT)
	{
		window.close();
		Util::stopSounds();
		return;
	}

	// 현재 메뉴에 있으면 아래 게임루프를 업데이트 하지 않음 (CLEAR_MENU는 위에 if문에서 이미 return 처리되고 있음)
	if (MenuManager::getInstance().isInMenu()) 
	{
		if (!_isGameResourceCleaned)
		{
			Game::getInstance().InitObject();  // 현재 world의 object 관련 정보를 담고있는 vector 초기화 (restart 후 이전 object들은 렌더링 되지 않도록)
			Physics::Shutdown(); // 기존 world 삭제처리
			//Resources::_textures.clear(); // texture는 게임 시작 시 최초 한번 초기화 하고 계속 사용하고 있으니 스테이지 종료 후 별도로 초기화하지 않음
			_isGameResourceCleaned = true;
			cout << "Game Resource Cleaned after game played !" << endl;
		}

		if (_menuState == MenuIndex::CLEAR_MENU) return;

		// 클리어 메뉴를 제외한 임의의 메뉴에 있다면 전용 bgm 출력
		if (Resources::_musics["Music_Menu.wav"].getStatus() == Music::Stopped) 
		{
			Resources::_musics["Music_Menu.wav"].play();
		}
		return;
	}
	

	if (player._isDead && Keyboard::isKeyPressed(Keyboard::Enter)) // 플레이어 사망상태에서 Enter 눌러서 재시작
		Restart(); 

	if (player._isDead) // player 사망상태에서는 update 하지 않음
	{	
		// 배경음 재생중이라면 정지 후 사망 bgm 출력
		if (Resources::_musics["Music_Game.wav"].getStatus() == Music::Playing) 
		{
			Resources::_musics["Music_Game.wav"].stop();	
			Resources::_sounds["Dead.wav"].play();
		}
		return;
	}

	Physics::Update(deltaTime);
	player.Update(deltaTime);
	_enemyPool->Update(deltaTime);
	for (Object* object : _objects)
		object->Update(deltaTime);

	camera._position = player._position;
}

void Game::Render(Renderer& renderer)
{
	// 메뉴에 있으면 메뉴만 렌더링 하고 return
	if (MenuManager::getInstance().isInMenu())
	{
		renderer._target.setView(renderer._target.getDefaultView()); // 메뉴 UI 표시를 위해 window default view로 회귀
		MenuManager::getInstance().render(renderer);
		return;
	}

	renderer._target.setView(camera.getView(renderer._target.getSize()));

	float backGroundPositionX = _mapBound.left + _mapBound.width / 2;
	float backGroundPositionY = _mapBound.top + _mapBound.height / 2;

	renderer.Draw(Resources::_textures["sky.png"], Vector2f(backGroundPositionX, backGroundPositionY), Vector2f(_mapBound.width, _mapBound.height)); // 배경화면 그리기
	gameMap.Draw(renderer);
	player.Draw(renderer);
	_enemyPool->Render(renderer);

	for (Object* object : _objects)
		object->Render(renderer);

	if (MAP_IS_DEBUG_DRAW)
		Physics::DebugDraw(renderer); 
}

void Game::RenderUI(Renderer& renderer)
{
	if (MenuManager::getInstance().isInMenu()) return;

	renderer._target.setView(camera.getUIView());

	// 판정 퍼센트 렌더링
	playerJudgementPercentageText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 1.f)); // view 중심 (0, 0)으로부터 계산되는 position, 왼쪽 위로 설정됨
	playerJudgementPercentageText.setString("Judgement: " + Util::floatToString(player.getJudgementPercentage(), 2) + "%");
	renderer._target.draw(playerJudgementPercentageText);

	// 판정등급 렌더링
	playerJudgementText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 5.f));
	playerJudgementText.setString(player.getJudgementCurrent());
	renderer._target.draw(playerJudgementText);

	// 코인 획득 수 렌더링
	playerCoinText.setPosition(Vector2f(camera.getViewSize().x / 2.f, -camera.getViewSize().y / 2.f) + Vector2f(-13.f, 1.f));
	playerCoinText.setString("Coin: " + to_string(Game::getInstance().getPlayer().getCoin()));
	renderer._target.draw(playerCoinText);

	// 플레이어 사망 시 배경화면 설정
	if (player._isDead)
	{
		backgroundWhenPaused.setScale(camera.getViewSize());
		renderer._target.draw(backgroundWhenPaused); 

		//gameOverText.setPosition(Vector2f(0.f, camera.getViewSize().y / 2.f) + Vector2f(0.f, -5.f));
		gameOverText.setString("Game Over!\nPress Enter to Restart");
		// Text origin을 중심으로 배치
		FloatRect textBounds = gameOverText.getLocalBounds();
		gameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
		renderer._target.draw(gameOverText);
	}

	// 게임 정지 시 선택지 제공
	if (_paused)
	{
		backgroundWhenPaused.setScale(camera.getViewSize());
		renderer._target.draw(backgroundWhenPaused);

		// Continue
		continueText.setPosition(Vector2f(0.f, - camera.getViewSize().y / 2.f) + Vector2f(0.f, 20.f));
		renderer._target.draw(continueText);

		// Restart
		restartText.setPosition(Vector2f(0.f, -camera.getViewSize().y / 2.f) + Vector2f(0.f, 30.f));
		renderer._target.draw(restartText);

		// Exit
		exitText.setPosition(Vector2f(0.f, -camera.getViewSize().y / 2.f) + Vector2f(0.f, 40.f));
		renderer._target.draw(exitText);

		// 왼쪽 마우스를 누른 경우
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			const RenderWindow* window = dynamic_cast<RenderWindow*>(&renderer._target);
			if (!window) return;
			Vector2f mousePos = renderer._target.mapPixelToCoords(Mouse::getPosition(*window)); // 마우스 위치 가져오기

			// Continue 클릭 처리 => 다시 게임으로 돌아감
			if (continueText.getGlobalBounds().contains(mousePos))
			{
				_paused = false;
				return;
			}

			// Restart 클릭 처리 => 게임을 재시작함
			if (restartText.getGlobalBounds().contains(mousePos))
			{
				_paused = false;
				Restart();
				return;
			}

			// Exit 클릭 처리 => 스테이지 선택 메뉴로 돌아감
			if (exitText.getGlobalBounds().contains(mousePos))
			{
				_paused = false;
				Resources::_musics["Music_Game.wav"].stop();
				setMenuState(MenuIndex::STAGE_MENU);
				return;
			}
			
			
		}
	}
}

void Game::setMapBound(FloatRect mapBound)
{
	_mapBound = mapBound;
}

FloatRect Game::getMapBound()
{
	return _mapBound;
}

void Game::DeleteObject(Object* object)
{
	// object iterator가 vector에 존재하면 동적할당 delete 및 vector에서 삭제처리
	const vector<Object*>::iterator& it = find(_objects.begin(), _objects.end(), object);
	if (it != _objects.end())
	{
		delete *it;
		_objects.erase(it);
	}
}

void Game::InitObject()
{
	if (!Physics::world) return; // 이미 월드가 삭제되어 있으면 return 처리

	// object 관리하는 모든 것들 초기화

	Physics::bodiesToDestroy.clear();
	Physics::bodiesSetEnabled.clear();

	for (Object* object : _objects)
		delete object; // 소멸자 호출 + 메모리 정리
	
	_objects.clear();
}
