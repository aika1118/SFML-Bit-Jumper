#include "Game.h"


Game::Game() // 참조자 멤버변수 초기화
	:gameMap(Map::getInstance()), camera(Camera::getInstance())
{
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

	//for (const auto& file : filesystem::directory_iterator("./resources/sounds/")) // sound 불러오기
	//{
	//	if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
	//		Resources::_sounds[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map에 texture 정보 저장
	//}

	//_mapImage.loadFromFile(MAP_STAGE_1);
	//_mapBound = FloatRect(0.f, 0.f, (float)_mapImage.getSize().x, (float)_mapImage.getSize().y); // 현재 mapBound 계산 (view가 맵 경계 벗어나지 않도록 하는 작업)

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

	backgroundWhenPaused.setSize(Vector2f(1.f, 1.f));
	backgroundWhenPaused.setFillColor(Color(0, 0, 0, 150)); // 알파값만 조정
	backgroundWhenPaused.setOrigin(0.5f, 0.5f); // origin을 중심으로 맞추기

	MenuManager::getInstance().init(window);

	InitSkill();

	Restart();
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

void Game::Restart() // Begin할 때의 Restart()가 뭔가 중복되는 것 같아서 코드 정리 필요
{
	InitObject();  // 현재 world의 object 관련 정보를 담고있는 vector 초기화 (restart 후 이전 object들은 렌더링 되지 않도록)
	Physics::Init(); // 기존 world 초기화

	player = Player(); // 임시 객체를 생성하고, 복사 대입 연산자로 새로 초기화
	gameMap.CreateFromImage(_mapImage, _objects); // map 따라 player 위치 및 object 생성

	player._position = _playerPosition;
	player.Begin();

	for (Object* object : _objects)
		object->Begin();
}

void Game::Update(float deltaTime, RenderWindow& window)
{
	if (_menuState == MenuIndex::EXIT)
	{
		window.close();
		return;
	}

	if (MenuManager::getInstance().isInMenu()) return;
	

	if (player._isDead && Keyboard::isKeyPressed(Keyboard::Enter)) // 플레이어 사망상태에서 Enter 눌러서 재시작
		Restart(); 

	if (player._isDead) // player 사망상태에서는 update 하지 않음
		return;

	Physics::Update(deltaTime);
	player.Update(deltaTime);
	for (Object* object : _objects)
		object->Update(deltaTime);

	camera._position = player._position;
}

void Game::Render(Renderer& renderer)
{
	if (MenuManager::getInstance().isInMenu())
	{
		MenuManager::getInstance().render(renderer);
		return;
	}

	renderer._target.setView(camera.getView(renderer._target.getSize()));

	float backGroundPositionX = _mapBound.left + _mapBound.width / 2;
	float backGroundPositionY = _mapBound.top + _mapBound.height / 2;

	renderer.Draw(Resources::_textures["sky.png"], Vector2f(backGroundPositionX, backGroundPositionY), Vector2f(_mapBound.width, _mapBound.height)); // 배경화면 그리기
	gameMap.Draw(renderer);
	player.Draw(renderer);

	for (Object* object : _objects)
		object->Render(renderer);

	if (MAP_IS_DEBUG_DRAW)
		Physics::DebugDraw(renderer); 
}

void Game::RenderUI(Renderer& renderer)
{
	if (MenuManager::getInstance().isInMenu()) return;

	renderer._target.setView(camera.getUIView());

	playerJudgementPercentageText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 1.f)); // view 중심 (0, 0)으로부터 계산되는 position, 왼쪽 위로 설정됨
	playerJudgementPercentageText.setString("Judgement: " + Util::floatToString(player.getJudgementPercentage(), 2) + "%");
	renderer._target.draw(playerJudgementPercentageText);

	playerJudgementText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 5.f));
	playerJudgementText.setString(player.getJudgementCurrent());
	renderer._target.draw(playerJudgementText);

	// 게임 일시정지 되었을 때 배경화면 지정
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
	// object 관리하는 모든 것들 초기화

	Physics::bodiesToDestroy.clear();

	for (Object* object : _objects)
		delete object;
	
	_objects.clear();
}
