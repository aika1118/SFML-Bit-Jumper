#include "Game.h"


Game::Game() // ������ ������� �ʱ�ȭ
	:gameMap(Map::getInstance()), camera(Camera::getInstance())
{
}

Game& Game::getInstance()
{
	static Game instance; // �ν��Ͻ��� ���� ȣ�� �ÿ��� ������
	return instance;
}


void Game::Begin(const RenderWindow& window)
{
	for (const auto& file : filesystem::directory_iterator("./resources/textures/")) // �ش� ��ο� �ִ� ��� texture �ҷ�����
	{
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg"))
			Resources::_textures[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map�� texture ���� ����
	}

	for (const auto& file : filesystem::directory_iterator("./resources/sounds/")) // sound �ҷ�����
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
			Resources::_sounds[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map�� texture ���� ����
	}

	_mapImage.loadFromFile(MAP_STAGE_1);
	_mapBound = FloatRect(0.f, 0.f, (float)_mapImage.getSize().x, (float)_mapImage.getSize().y); // ���� mapBound ��� (view�� �� ��� ����� �ʵ��� �ϴ� �۾�)

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

void Game::Restart()
{
	InitObject();  // ���� world�� object ���� ������ ����ִ� vector �ʱ�ȭ (restart �� ���� object���� ������ ���� �ʵ���)
	Physics::Init(); // ���� world �ʱ�ȭ

	player = Player(); // �ӽ� ��ü�� �����ϰ�, ���� ���� �����ڷ� ���� �ʱ�ȭ
	gameMap.CreateFromImage(_mapImage, _objects); // map ���� player ��ġ �� object ����

	player._position = _playerPosition;
	player.Begin();

	for (Object* object : _objects)
		object->Begin();
}

void Game::Update(float deltaTime)
{
	if (player._isDead) // �÷��̾� ��� �� �����
		Restart();

	Physics::Update(deltaTime);
	player.Update(deltaTime);
	for (Object* object : _objects)
		object->Update(deltaTime);

	camera._position = player._position;
}

void Game::Render(Renderer& renderer)
{
	renderer._target.setView(camera.getView(renderer._target.getSize()));

	float backGroundPositionX = _mapBound.left + _mapBound.width / 2;
	float backGroundPositionY = _mapBound.top + _mapBound.height / 2;

	renderer.Draw(Resources::_textures["sky.png"], Vector2f(backGroundPositionX, backGroundPositionY), Vector2f(_mapBound.width, _mapBound.height)); // ���ȭ�� �׸���
	gameMap.Draw(renderer);
	player.Draw(renderer);

	for (Object* object : _objects)
		object->Render(renderer);

	if (MAP_IS_DEBUG_DRAW)
		Physics::DebugDraw(renderer); 
}

void Game::RenderUI(Renderer& renderer)
{
	renderer._target.setView(camera.getUIView());

	playerJudgementPercentageText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 1.f)); // view �߽� (0, 0)���κ��� ���Ǵ� position, ���� ���� ������
	playerJudgementPercentageText.setString("Judgement: " + Util::floatToString(player.getJudgementPercentage(), 2) + "%");
	renderer._target.draw(playerJudgementPercentageText);

	playerJudgementText.setPosition(-camera.getViewSize() / 2.f + Vector2f(2.f, 5.f));
	playerJudgementText.setString(player.getJudgementCurrent());
	renderer._target.draw(playerJudgementText);
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
	// object �����ϴ� ��� �͵� �ʱ�ȭ

	Physics::bodiesToDestroy.clear();

	for (Object* object : _objects)
		delete object;
	
	_objects.clear();
}
