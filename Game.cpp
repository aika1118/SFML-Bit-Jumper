#include "Game.h"

Game::Game() // �̱��� ��ü (���� �ν��Ͻ�) ���� �� �⺻ ������ �ʿ�
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
	Physics::Init();

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

	_mapImage.loadFromFile("./resources/images/map.png");

	player._position = gameMap.CreateFromImage(_mapImage, _objects);
	player.Begin();
}

void Game::Update(float deltaTime)
{
	Physics::Update(deltaTime);
	player.Update(deltaTime);
	camera._position = player._position;
}

void Game::Render(Renderer& renderer)
{
	renderer._target.setView(camera.getView(renderer._target.getSize()));
	gameMap.Draw(renderer);
	player.Draw(renderer);

	Physics::DebugDraw(renderer);
}
