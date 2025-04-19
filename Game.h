// File: Game.h
// Purpose: ���� �ʱ�ȭ, ������Ʈ, ������ ����

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
	static Game& getInstance(); // �̱���

	void Begin(RenderWindow& window); // ���� ���� �� �ʱ�ȭ �۾��� ����
	void Update(float deltaTime, RenderWindow& window); // ���� ������Ʈ �۾�
	void Render(Renderer& renderer); // ���� ������ �۾�
	void RenderUI(Renderer& renderer); // UI ������ �۾�
	void Restart(); // ���� �����

	void setMapBound(FloatRect mapBound); // ���� �� �ٿ�� ����
	FloatRect getMapBound(); // ���� �� �ٿ�� ��ȯ

	void DeleteObject(Object* object); // ���� �� object ����
	void InitObject(); // // object �����ϴ� ��� �͵� �ʱ�ȭ

	Vector2f _playerPosition;
	int _savePositionX = PLAYER_NO_SAVE_POSITION;
	int _savePositionY = PLAYER_NO_SAVE_POSITION;

	void InitSkill();

	Vector2f getPlayerPosition(); // player�� ���� position ��ȯ
	float getPlayerAngle(); // player�� ���� angle ��ȯ
	bool getPlayerFacingLeft(); // player�� �ٶ󺸴� ���� ��ȯ
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
	bool isServerConnected();

	Client* getClient();
	io_context& getIoContext();

	int getUid() const;
	void setUid(int uid);

	string getUsername();
	EnemyPool* GetEnemyPool();


private:
	Game(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	Game(const Game&) = delete; // ��������� ����
	Game& operator=(const Game&) = delete; // ���Կ����� ����

	Camera& camera;
	Map& gameMap;
	Image _mapImage; // ���� ��
	vector<Object*> _objects; // ���� �� object ����
	Player player;

	Font font;
	Text playerJudgementPercentageText;
	Text playerJudgementText;
	Text playerCoinText;
	Text gameOverText;

	RectangleShape backgroundWhenPaused; // �Ͻ������� ���� ���

	FloatRect _mapBound;

	int _menuState = MenuIndex::MAIN_MENU;
	int _stageSelected = -1;

	unordered_map<int, int> _playerCurrentClearStages;
	unordered_map<int, unordered_map<int, float>> _playerStageScores;
	
	Client* client = nullptr; // Client ��ü ����
	io_context io_context;

	int _uid = SETTING_UID_NOT_INITED; // ���� ���� UID 
	string _username; // ���� �̸� (uid�� �÷��̾ �����ϰ� �ֱ� ������ username�� �ٸ� ������ �ߺ��ǵ� �������)

	bool _isUidInited = false; // _uid�� �ʱ�ȭ �ߴ��� ����
	bool _isServerConnected = true;

	mutex playerCurrentClearStages_mutex_; // _playerCurrentClearStages �ڿ��� ��Ʈ��ũ �����尡 �ݹ� �Լ����� ������ �����ϰ� ���� �����忡�� �б� �� �۾��� �ֱ� ������ lock���� ��ȣ
	EnemyPool* _enemyPool = nullptr;
};