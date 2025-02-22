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
#include "Client.h"
#include <boost/asio.hpp>
#include <thread>

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
	int getStageSelected();
	void setStageSelected(int stage);
	void setMapImage(int stage);
	void setMenuState(int menuState);

	int getPlayerCurrentClearStage(int id);
	void setPlayerCurrentClearStage(int id, int stage);

	float getPlayerStageScore(int id, int stage);
	void setPlayerStageScore(int id, int stage, float score);

	Client* getClient();
	io_context& getIoContext();

	int getUid();
	string getUsername();


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
	int _stageSelected = -1;

	unordered_map<int, int> _playerCurrentClearStages;
	unordered_map<int, unordered_map<int, float>> _playerStageScores;
	
	Client* client; // Client 객체 참조
	io_context io_context;

	int _uid = SETTING_UID_NOT_INITED; // 유저 고유 UID
	string _username; // 유저 이름 (uid로 플레이어를 구분하고 있기 때문에 username은 다른 유저와 중복되도 상관없음)

	bool _isUidInited = false; // _uid를 초기화 했는지 저장
};