#include "Game.h"


Game::Game() // ������ ������� �ʱ�ȭ
	:gameMap(Map::getInstance()), camera(Camera::getInstance())
{
	if (Util::checkServerConnection() == false)
	{
		cout << "Offline Mode !" << endl;
		_isServerConnected = false;
		return; // ���� ������ ������ �ʴ� ��� (config.h���� �� ����)
	}

	try
	{
		client = new Client(io_context, SERVER_IP, SERVER_PORT); // Ŭ���̾�Ʈ ����
		cout << "Client Connected !" << endl;
	}
	catch (const std::exception& e)
	{
		cerr << "Exception: " << e.what() << "\n"; // ����ó��
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
	static Game instance; // �ν��Ͻ��� ���� ȣ�� �ÿ��� ������
	return instance;
}


void Game::Begin(RenderWindow& window)
{
	for (const auto& file : filesystem::directory_iterator("./resources/textures/")) // �ش� ��ο� �ִ� ��� texture �ҷ�����
	{
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg"))
			Resources::_textures[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map�� texture ���� ����
	}

	//for (const auto& file : filesystem::directory_iterator("./resources/sounds/")) // sound �ҷ�����
	//{
	//	if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
	//		Resources::_sounds[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map�� texture ���� ����
	//}

	//_mapImage.loadFromFile(MAP_STAGE_1);
	//_mapBound = FloatRect(0.f, 0.f, (float)_mapImage.getSize().x, (float)_mapImage.getSize().y); // ���� mapBound ��� (view�� �� ��� ����� �ʵ��� �ϴ� �۾�)

	if (!font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
	{
		cerr << "font load error!" << endl;
		return;
	}

		
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

	playerCoinText.setFont(font);
	playerCoinText.setFillColor(Color::White);
	playerCoinText.setOutlineColor(Color::Black);
	playerCoinText.setOutlineThickness(1.f);
	playerCoinText.setScale(UI_CHARACTER_SCALE/1.2f, UI_CHARACTER_SCALE/1.2f);

	backgroundWhenPaused.setSize(Vector2f(1.f, 1.f));
	backgroundWhenPaused.setFillColor(Color(0, 0, 0, 150)); // ���İ��� ����
	backgroundWhenPaused.setOrigin(0.5f, 0.5f); // origin�� �߽����� ���߱�

	MenuManager::getInstance().init(window);

	// �������� �ȵǾ������� ���� �������� ��Ȳ�� �⺻������ �ʱ�ȭ (_uid�� �̹� SETTING_UID_NOT_INITED ������ �ʱ�ȭ�Ǿ�����)
	if (_isServerConnected == false) setPlayerCurrentClearStage(getUid(), PLAYER_DEFAULT_STAGE);

	InitSkill();

	Restart();
}

void Game::InitSkill()
{
	// melee skill �ʱ�ȭ
	int meleeStart = (int)SkillList::MELEE_START;
	int meleeEnd = (int)SkillList::MELEE_END;

	for (int skillId = meleeStart + 1; skillId < meleeEnd; ++skillId)
	{
		Skill* skill = new SkillMeleeAttack(skillId); // skillId�� ���� ��ų ������ ��ü ����
		SkillManager::getInstance().AddSkill(skillId, skill); // skillId�� �����Ͽ� skill�� unordered_map�� �߰�
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
	_mapImage.loadFromFile(Map::getInstance().getMapImages(stage)); // load ������ �� ����ó�� �ʿ�
	_mapBound = FloatRect(0.f, 0.f, (float)_mapImage.getSize().x, (float)_mapImage.getSize().y); // ���� mapBound ��� (view�� �� ��� ����� �ʵ��� �ϴ� �۾�)
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

void Game::Restart() // Begin�� ���� Restart()�� ���� �ߺ��Ǵ� �� ���Ƽ� �ڵ� ���� �ʿ� (world�� Game �����ڿ��� Init�ϰ� Restart()���� �� Init �Ǵµ� �Ͽ� ���� ������ Ȯ��)
{
	cout << "Restart() Called !" << endl;
	InitObject();  // ���� world�� object ���� ������ ����ִ� vector �ʱ�ȭ (restart �� ���� object���� ������ ���� �ʵ���)
	Physics::Init(); // ���� world �ʱ�ȭ
	_enemyPool->Reset(); // enemyPool �ʱ�ȭ

	player = Player(); // �ӽ� ��ü�� �����ϰ�, ���� ���� �����ڷ� ���� �ʱ�ȭ
	gameMap.CreateFromImage(_mapImage, _objects); // map ���� player ��ġ �� object ����

	player._position = _playerPosition;
	player.Begin();

	for (Object* object : _objects)
		object->Begin();
}

void Game::Update(float deltaTime, RenderWindow& window)
{
	if (_isServerConnected && !_isUidInited) // uid�� �ʱ�ȭ �ɶ����� �ش� if���ǿ� ��� �ɸ��Ե�
	{
		// ���� �г��� ���� �޴��� �ִٸ� return
		if (_menuState == MenuIndex::MAKE_USERNAME_MENU) return;

		_uid = Util::getUID();

		// ���� �������Ͽ� uid�� ���� ���
		if (_uid == SETTING_UID_NOT_INITED) setMenuState(MenuIndex::MAKE_USERNAME_MENU); // �г��� ���� �޴��� �̵�

		else // �������� uid�� ���� ���
		{
			_username = Util::getUserName(_uid); // uid�� ���� ��� username�� get �ϱ�
			_menuState = MenuIndex::MAIN_MENU; // ���� �޴��� �̵�

			// uid�� �ش��ϴ� �ֽ� clear stage�� �Ʒ� const ������ �ʱ�ȭ (���� db�� �ޱ����� �ε����� �˸��� ó���� �ϱ� ���� in StageMenu.cpp)
			setPlayerCurrentClearStage(_uid, SETTING_PLAYER_CURRENT_CLEAR_STAGE_NOT_INITED);
			
			// uid�� �ش��ϴ� �ֽ� clear stage ���� �޾ƿ���
			if (Game::getInstance().isServerConnected())
				client->send_packet_async(PACKET_READ_MAX_CLEAR_STAGE, to_string(_uid),
					[](const string& response) { // �ݹ� ����
						cout << "PACKET_READ_MAX_CLEAR_STAGE callback!" << endl;
						// uid�� �ش��ϴ� ���� ���� Ŭ�����ߴ� stage ����
						Game::getInstance().setPlayerCurrentClearStage(Game::getInstance().getUid(), stoi(response));
						return;
					}
				);
			else // Update() if ���ǿ��� ���������� Ȯ�������� Ȥ�� ���� ����ó��
				setPlayerCurrentClearStage(_uid, PLAYER_DEFAULT_STAGE);

			_isUidInited = true;
		}
	}

	MenuManager::getInstance().setCurrentMenu(_menuState);

	if (_menuState == MenuIndex::EXIT)
	{
		window.close();
		return;
	}

	// ���� ���� �� Ŭ����޴��� ���� �� �޸� ���� �۾� ��
	if (_menuState == MenuIndex::CLEAR_MENU)
	{
		Game::getInstance().InitObject();  // ���� world�� object ���� ������ ����ִ� vector �ʱ�ȭ (restart �� ���� object���� ������ ���� �ʵ���)
		Physics::Shutdown(); // ���� world ����ó��
		//Resources::_textures.clear(); // texture�� ���� ���� �� ���� �ѹ� �ʱ�ȭ �ϰ� ��� ����ϰ� ������ �������� ���� �� ������ �ʱ�ȭ���� ����
		return;
	}

	if (MenuManager::getInstance().isInMenu()) return;
	

	if (player._isDead && Keyboard::isKeyPressed(Keyboard::Enter)) // �÷��̾� ������¿��� Enter ������ �����
		Restart(); 

	if (player._isDead) // player ������¿����� update ���� ����
		return;

	Physics::Update(deltaTime);
	player.Update(deltaTime);
	_enemyPool->Update(deltaTime);
	for (Object* object : _objects)
		object->Update(deltaTime);

	camera._position = player._position;
}

void Game::Render(Renderer& renderer)
{
	if (MenuManager::getInstance().isInMenu())
	{
		renderer._target.setView(renderer._target.getDefaultView()); // �޴� UI ǥ�ø� ���� window default view�� ȸ��
		MenuManager::getInstance().render(renderer);
		return;
	}

	renderer._target.setView(camera.getView(renderer._target.getSize()));

	float backGroundPositionX = _mapBound.left + _mapBound.width / 2;
	float backGroundPositionY = _mapBound.top + _mapBound.height / 2;

	renderer.Draw(Resources::_textures["sky.png"], Vector2f(backGroundPositionX, backGroundPositionY), Vector2f(_mapBound.width, _mapBound.height)); // ���ȭ�� �׸���
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

	// ���� �ۼ�Ʈ ������
	playerJudgementPercentageText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 1.f)); // view �߽� (0, 0)���κ��� ���Ǵ� position, ���� ���� ������
	playerJudgementPercentageText.setString("Judgement: " + Util::floatToString(player.getJudgementPercentage(), 2) + "%");
	renderer._target.draw(playerJudgementPercentageText);

	// ������� ������
	playerJudgementText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 5.f));
	playerJudgementText.setString(player.getJudgementCurrent());
	renderer._target.draw(playerJudgementText);

	// ���� ȹ�� �� ������
	playerCoinText.setPosition(Vector2f(camera.getViewSize().x / 2.f, -camera.getViewSize().y / 2.f) + Vector2f(-13.f, 1.f));
	playerCoinText.setString("Coin: " + to_string(Game::getInstance().getPlayer().getCoin()));
	renderer._target.draw(playerCoinText);

	// ���� �Ͻ����� �Ǿ��� �� ���ȭ�� ����
	if (player._isDead)
	{
		backgroundWhenPaused.setScale(camera.getViewSize());
		renderer._target.draw(backgroundWhenPaused); 

		//gameOverText.setPosition(Vector2f(0.f, camera.getViewSize().y / 2.f) + Vector2f(0.f, -5.f));
		gameOverText.setString("Game Over!\nPress Enter to Restart");
		// Text origin�� �߽����� ��ġ
		FloatRect textBounds = gameOverText.getLocalBounds();
		gameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
		renderer._target.draw(gameOverText);
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
	// object iterator�� vector�� �����ϸ� �����Ҵ� delete �� vector���� ����ó��
	const vector<Object*>::iterator& it = find(_objects.begin(), _objects.end(), object);
	if (it != _objects.end())
	{
		delete *it;
		_objects.erase(it);
	}
}

void Game::InitObject()
{
	if (!Physics::world) return; // �̹� ���尡 �����Ǿ� ������ return ó��

	// object �����ϴ� ��� �͵� �ʱ�ȭ

	Physics::bodiesToDestroy.clear();
	Physics::bodiesSetEnabled.clear();

	for (Object* object : _objects)
		delete object; // �Ҹ��� ȣ�� + �޸� ����
	
	_objects.clear();
}
