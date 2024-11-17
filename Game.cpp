#include "Game.h"

Game::Game() // 싱글톤 객체 (정적 인스턴스) 생성 시 기본 생성자 필요
	:gameMap(Map::getInstance()), camera(Camera::getInstance())
{
}

Game& Game::getInstance()
{
	static Game instance; // 인스턴스는 최초 호출 시에만 생성됨
	return instance;
}

void Game::Begin(const RenderWindow& window)
{
	Physics::Init();

	for (const auto& file : filesystem::directory_iterator("./resources/textures/")) // 해당 경로에 있는 모든 texture 불러오기
	{
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg"))
			Resources::_textures[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map에 texture 정보 저장
	}

	for (const auto& file : filesystem::directory_iterator("./resources/sounds/")) // sound 불러오기
	{
		if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav"))
			Resources::_sounds[file.path().filename().string()].loadFromFile(file.path().string()); // unordered_map에 texture 정보 저장
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
