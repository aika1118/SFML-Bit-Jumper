// File: Game.h
// Purpose: 게임 초기화, 업데이트, 렌더링 관리

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include "Renderer.h"
#include "Map.h"
#include "Camera.h"
#include "Physics.h"
#include "Player.h"
#include "SkillManager.h"
#include "SkillMeleeAttack.h"
#include "Util.h"
#include "MenuManager.h"

using namespace sf;
using namespace std;


class Game
{
public:
	static Game& getInstance(); // 싱글톤

	void Begin(RenderWindow& window); // 게임 시작 전 초기화 작업을 수행
	void Update(float deltaTime, RenderWindow& window); // 게임 업데이트 작업
	void Render(Renderer& renderer); // 게임 렌더링 작업
	void RenderUI(Renderer& renderer); // UI 렌더링 작업
	void Restart(); // 게임 재시작

	void setMapBound(FloatRect mapBound); // 현재 맵 바운드 설정
	FloatRect getMapBound(); // 현재 맵 바운드 반환

	void DeleteObject(Object* object); // 게임 내 object 삭제
	void InitObject(); // // object 관리하는 모든 것들 초기화

	Vector2f _playerPosition;
	int _savePositionX = 0;
	int _savePositionY = 0;

	void InitSkill();

	Vector2f getPlayerPosition(); // player의 현재 position 반환
	float getPlayerAngle(); // player의 현재 angle 반환
	bool getPlayerFacingLeft(); // player의 바라보는 방향 반환
	Player& getPlayer(); 
	int& getMenuState();


private:
	Game(); // 외부에서 생성자 호출 불가하도록 설정
	Game(const Game&) = delete; // 복사생성자 삭제
	Game& operator=(const Game&) = delete; // 대입연산자 삭제

	Camera& camera;
	Map& gameMap;
	Image _mapImage; // 게임 맵
	vector<Object*> _objects; // 게임 내 object 관리
	Player player;

	Font font;
	Text playerJudgementPercentageText;
	Text playerJudgementText;
	Text gameOverText;

	RectangleShape backgroundWhenPaused; // 일시정지할 때의 배경

	FloatRect _mapBound;

	int _menuState = MenuIndex::MAIN_MENU;
};