// File: Game.h
// Purpose: 게임 초기화, 업데이트, 렌더링 관리

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include "Renderer.h"
#include "Map.h"
#include "Camera.h"

using namespace sf;
using namespace std;

class Game
{
public:
	static Game& getInstance(); // 싱글톤

	void Begin(const RenderWindow& window); // 게임 시작 전 초기화 작업을 수행
	void Update(float deltaTime); // 게임 업데이트 작업
	void Render(Renderer& renderer); // 게임 렌더링 작업

private:
	Game(); // 외부에서 생성자 호출 불가하도록 설정
	Game(const Game&) = delete; // 복사생성자 삭제
	Game& operator=(const Game&) = delete; // 대입연산자 삭제

	Camera& camera;
	Map& gameMap;
	Image _mapImage; // 게임 배경화면
	vector<Object*> _objects; // 게임 내 object 관리
};