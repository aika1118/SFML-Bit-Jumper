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
#include <mutex>
#include "EnemyPool.h"

using namespace sf;
using namespace std;


class Game
{
public:
	/**
	 * @brief 싱글톤 인스턴스를 가져옵니다.
	 *
	 * 싱글톤 인스턴스를 반환합니다.
	 *
	 * @return Game& 싱글톤 인스턴스
	 */
	static Game& getInstance();
	/**
	 * @brief 게임을 초기화합니다.
	 *
	 * 게임을 시작하기 전에 필요한 초기화를 수행합니다.
	 *
	 * @param window 렌더링에 사용할 RenderWindow 객체
	 */
	void Begin(RenderWindow& window);
	/**
	 * @brief 게임 상태를 업데이트합니다.
	 * 
	 * 이전 프레임 이후 경과된 시간을 기준으로 게임 상태를 업데이트합니다.
	 * 
	 * @param deltaTime 이전 프레임 이후 경과된 시간
	 * @param window 렌더링에 사용할 RenderWindow 객체
	 */
	void Update(float deltaTime, RenderWindow& window); 
	/**
	 * @brief 게임을 렌더링합니다.
	 *
	 * 제공된 Renderer 객체를 사용하여 게임을 렌더링합니다.
	 *
	 * @param renderer 렌더링에 사용할 Renderer 객체
	 */
	void Render(Renderer& renderer);
	/**
	 * @brief 게임의 UI를 렌더링합니다.
	 *
	 * 제공된 Renderer 객체를 사용하여 게임의 UI를 렌더링합니다.
	 *
	 * @param renderer 렌더링에 사용할 Renderer 객체
	 */
	void RenderUI(Renderer& renderer);
	/**
	 * @brief 게임을 재시작합니다.
	 *
	 * 게임을 처음부터 다시 시작합니다.
	 */
	void Restart();
	/**
	 * @brief 현재 맵 경계를 설정합니다.
	 *
	 * 현재 맵 경계를 지정된 FloatRect로 설정합니다.
	 *
	 * @param mapBound 새로운 맵 경계
	 */
	void setMapBound(FloatRect mapBound); 
	/**
	 * @brief 현재 맵 경계를 가져옵니다.
	 *
	 * 현재 맵 경계를 반환합니다.
	 *
	 * @return FloatRect 현재 맵 경계
	 */
	FloatRect getMapBound(); 
	/**
	 * @brief 게임에서 object를 삭제합니다.
	 *
	 * 지정된 object를 게임에서 삭제합니다.
	 *
	 * @param 삭제할 object
	 */
	void DeleteObject(Object* object); 
	/**
	 * @brief 게임의 모든 object를 초기화합니다.
	 *
	 * 게임의 모든 object를 초기화합니다.
	 */
	void InitObject(); 
	/**
	 * @brief 플레이어의 스킬을 초기화합니다.
	 *
	 * 플레이어의 스킬을 초기화합니다.
	 */
	void InitSkill();
	/**
	 * @brief 플레이어의 현재 위치를 가져옵니다.
	 *
	 * 플레이어의 현재 위치를 반환합니다.
	 *
	 * @return Vector2f 플레이어의 현재 위치
	 */
	Vector2f getPlayerPosition(); 
	/**
	 * @brief 플레이어의 현재 각도를 가져옵니다.
	 *
	 * 플레이어의 현재 각도를 반환합니다.
	 *
	 * @return float 플레이어의 현재 각도
	 */
	float getPlayerAngle();
	/**
	 * @brief 플레이어가 왼쪽을 바라보고 있는지 확인합니다.
	 *
	 * 플레이어가 왼쪽을 바라보고 있는지 여부를 반환합니다.
	 *
	 * @return bool 플레이어가 왼쪽을 바라보고 있는지 여부
	 */
	bool getPlayerFacingLeft(); 
	/**
	 * @brief 플레이어 객체를 가져옵니다.
	 *
	 * 플레이어 객체를 반환합니다.
	 *
	 * @return Player& 플레이어 객체
	 */
	Player& getPlayer(); 
	/**
	 * @brief 현재 메뉴 상태를 가져옵니다.
	 *
	 * 현재 메뉴 상태를 반환합니다.
	 *
	 * @return int 현재 메뉴 상태
	 */
	int& getMenuState();
	/**
	 * @brief 현재 선택된 스테이지를 가져옵니다.
	 *
	 * 현재 선택된 스테이지를 반환합니다.
	 *
	 * @return int 현재 선택된 스테이지
	 */
	int getStageSelected();
	/**
	 * @brief 선택된 스테이지를 설정합니다.
	 *
	 * 선택된 스테이지를 설정합니다.
	 *
	 * @param stage 선택할 스테이지
	 */
	void setStageSelected(int stage);
	/**
	 * @brief 선택된 스테이지에 해당하는 맵 이미지를 설정합니다.
	 *
	 * 선택된 스테이지에 해당하는 맵 이미지를 설정합니다.
	 *
	 * @param stage 선택할 스테이지
	 */
	void setMapImage(int stage);
	/**
	 * @brief 현재 메뉴 상태를 설정합니다.
	 *
	 * 현재 메뉴 상태를 설정합니다.
	 *
	 * @param menuState 설정할 메뉴 상태
	 */
	void setMenuState(int menuState);
	/**
	 * @brief 플레이어의 현재 클리어한 스테이지를 가져옵니다.
	 *
	 * 플레이어의 현재 클리어한 스테이지를 반환합니다.
	 *
	 * @param id 플레이어의 ID
	 * @return int 플레이어의 현재 클리어한 스테이지
	 */
	int getPlayerCurrentClearStage(int id);
	/**
	 * @brief 플레이어의 클리어한 스테이지를 설정합니다.
	 *
	 * 플레이어의 클리어한 스테이지를 설정합니다.
	 *
	 * @param id 플레이어의 ID
	 * @param stage 설정할 클리어한 스테이지
	 */
	void setPlayerCurrentClearStage(int id, int stage);
	/**
	 * @brief 플레이어의 스테이지 점수를 가져옵니다.
	 *
	 * 플레이어의 스테이지 점수를 반환합니다.
	 *
	 * @param id 플레이어의 ID
	 * @param stage 스테이지
	 * @return float 플레이어의 스테이지 점수
	 */
	float getPlayerStageScore(int id, int stage);
	/**
	 * @brief 플레이어의 스테이지 점수를 설정합니다.
	 *
	 * 플레이어의 스테이지 점수를 설정합니다.
	 *
	 * @param id 플레이어의 ID
	 * @param stage 스테이지
	 * @param score 설정할 스테이지 점수
	 */
	void setPlayerStageScore(int id, int stage, float score);
	/**
	 * @brief 서버와의 연결 여부를 확인합니다.
	 *
	 * 서버와의 연결 여부를 반환합니다.
	 *
	 * @return bool 서버와의 연결 여부
	 */
	bool isServerConnected();
	/**
	 * @brief 클라이언트 객체를 가져옵니다.
	 *
	 * 클라이언트 객체를 반환합니다.
	 *
	 * @return Client* 클라이언트 객체
	 */
	Client* getClient();
	/**
	 * @brief IO 컨텍스트를 가져옵니다.
	 *
	 * IO 컨텍스트를 반환합니다.
	 *
	 * @return io_context& IO 컨텍스트
	 */
	io_context& getIoContext();
	/**
	 * @brief 사용자 UID를 가져옵니다.
	 *
	 * 사용자 UID를 반환합니다.
	 *
	 * @return int 사용자 UID
	 */
	int getUid() const;
	/**
	 * @brief 사용자 UID를 설정합니다.
	 *
	 * 사용자 UID를 설정합니다.
	 *
	 * @param 설정할 사용자 UID
	 */
	void setUid(int uid);
	/**
	 * @brief 사용자 이름을 가져옵니다.
	 *
	 * 사용자 이름을 반환합니다.
	 *
	 * @return string 사용자 이름
	 */
	string getUsername();
	/**
	 * @brief 적 풀 객체를 가져옵니다.
	 *
	 * 적 풀 객체를 반환합니다.
	 *
	 * @return EnemyPool* 적 풀 객체
	 */
	EnemyPool* GetEnemyPool();

	Vector2f _playerPosition; // 플레이어 위치
	int _savePositionX = PLAYER_NO_SAVE_POSITION; // 플레이어 세이브 포인트 위치 X
	int _savePositionY = PLAYER_NO_SAVE_POSITION; // 플레이어 세이브 포인트 위치 Y

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
	Text playerCoinText;
	Text gameOverText;

	RectangleShape backgroundWhenPaused; // 일시정지할 때의 배경

	FloatRect _mapBound;

	int _menuState = MenuIndex::MAIN_MENU;
	int _stageSelected = -1;

	unordered_map<int, int> _playerCurrentClearStages;
	unordered_map<int, unordered_map<int, float>> _playerStageScores;
	
	Client* client = nullptr; // Client 객체 참조
	io_context io_context;

	int _uid = SETTING_UID_NOT_INITED; // 유저 고유 UID 
	string _username; // 유저 이름 (uid로 플레이어를 구분하고 있기 때문에 username은 다른 유저와 중복되도 상관없음)

	bool _isUidInited = false; // _uid를 초기화 했는지 저장
	bool _isServerConnected = true;

	mutex playerCurrentClearStages_mutex_; // _playerCurrentClearStages 자원은 네트워크 쓰레드가 콜백 함수에서 수정을 진행하고 메인 쓰레드에서 읽기 등 작업이 있기 때문에 lock으로 보호
	EnemyPool* _enemyPool = nullptr;
};