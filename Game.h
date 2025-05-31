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
	/**
	 * @brief �̱��� �ν��Ͻ��� �����ɴϴ�.
	 *
	 * �̱��� �ν��Ͻ��� ��ȯ�մϴ�.
	 *
	 * @return Game& �̱��� �ν��Ͻ�
	 */
	static Game& getInstance();
	/**
	 * @brief ������ �ʱ�ȭ�մϴ�.
	 *
	 * ������ �����ϱ� ���� �ʿ��� �ʱ�ȭ�� �����մϴ�.
	 *
	 * @param window �������� ����� RenderWindow ��ü
	 */
	void Begin(RenderWindow& window);
	/**
	 * @brief ���� ���¸� ������Ʈ�մϴ�.
	 * 
	 * ���� ������ ���� ����� �ð��� �������� ���� ���¸� ������Ʈ�մϴ�.
	 * 
	 * @param deltaTime ���� ������ ���� ����� �ð�
	 * @param window �������� ����� RenderWindow ��ü
	 */
	void Update(float deltaTime, RenderWindow& window); 
	/**
	 * @brief ������ �������մϴ�.
	 *
	 * ������ Renderer ��ü�� ����Ͽ� ������ �������մϴ�.
	 *
	 * @param renderer �������� ����� Renderer ��ü
	 */
	void Render(Renderer& renderer);
	/**
	 * @brief ������ UI�� �������մϴ�.
	 *
	 * ������ Renderer ��ü�� ����Ͽ� ������ UI�� �������մϴ�.
	 *
	 * @param renderer �������� ����� Renderer ��ü
	 */
	void RenderUI(Renderer& renderer);
	/**
	 * @brief ������ ������մϴ�.
	 *
	 * ������ ó������ �ٽ� �����մϴ�.
	 */
	void Restart();
	/**
	 * @brief ���� �� ��踦 �����մϴ�.
	 *
	 * ���� �� ��踦 ������ FloatRect�� �����մϴ�.
	 *
	 * @param mapBound ���ο� �� ���
	 */
	void setMapBound(FloatRect mapBound); 
	/**
	 * @brief ���� �� ��踦 �����ɴϴ�.
	 *
	 * ���� �� ��踦 ��ȯ�մϴ�.
	 *
	 * @return FloatRect ���� �� ���
	 */
	FloatRect getMapBound(); 
	/**
	 * @brief ���ӿ��� object�� �����մϴ�.
	 *
	 * ������ object�� ���ӿ��� �����մϴ�.
	 *
	 * @param ������ object
	 */
	void DeleteObject(Object* object); 
	/**
	 * @brief ������ ��� object�� �ʱ�ȭ�մϴ�.
	 *
	 * ������ ��� object�� �ʱ�ȭ�մϴ�.
	 */
	void InitObject(); 
	/**
	 * @brief �÷��̾��� ��ų�� �ʱ�ȭ�մϴ�.
	 *
	 * �÷��̾��� ��ų�� �ʱ�ȭ�մϴ�.
	 */
	void InitSkill();
	/**
	 * @brief �÷��̾��� ���� ��ġ�� �����ɴϴ�.
	 *
	 * �÷��̾��� ���� ��ġ�� ��ȯ�մϴ�.
	 *
	 * @return Vector2f �÷��̾��� ���� ��ġ
	 */
	Vector2f getPlayerPosition(); 
	/**
	 * @brief �÷��̾��� ���� ������ �����ɴϴ�.
	 *
	 * �÷��̾��� ���� ������ ��ȯ�մϴ�.
	 *
	 * @return float �÷��̾��� ���� ����
	 */
	float getPlayerAngle();
	/**
	 * @brief �÷��̾ ������ �ٶ󺸰� �ִ��� Ȯ���մϴ�.
	 *
	 * �÷��̾ ������ �ٶ󺸰� �ִ��� ���θ� ��ȯ�մϴ�.
	 *
	 * @return bool �÷��̾ ������ �ٶ󺸰� �ִ��� ����
	 */
	bool getPlayerFacingLeft(); 
	/**
	 * @brief �÷��̾� ��ü�� �����ɴϴ�.
	 *
	 * �÷��̾� ��ü�� ��ȯ�մϴ�.
	 *
	 * @return Player& �÷��̾� ��ü
	 */
	Player& getPlayer(); 
	/**
	 * @brief ���� �޴� ���¸� �����ɴϴ�.
	 *
	 * ���� �޴� ���¸� ��ȯ�մϴ�.
	 *
	 * @return int ���� �޴� ����
	 */
	int& getMenuState();
	/**
	 * @brief ���� ���õ� ���������� �����ɴϴ�.
	 *
	 * ���� ���õ� ���������� ��ȯ�մϴ�.
	 *
	 * @return int ���� ���õ� ��������
	 */
	int getStageSelected();
	/**
	 * @brief ���õ� ���������� �����մϴ�.
	 *
	 * ���õ� ���������� �����մϴ�.
	 *
	 * @param stage ������ ��������
	 */
	void setStageSelected(int stage);
	/**
	 * @brief ���õ� ���������� �ش��ϴ� �� �̹����� �����մϴ�.
	 *
	 * ���õ� ���������� �ش��ϴ� �� �̹����� �����մϴ�.
	 *
	 * @param stage ������ ��������
	 */
	void setMapImage(int stage);
	/**
	 * @brief ���� �޴� ���¸� �����մϴ�.
	 *
	 * ���� �޴� ���¸� �����մϴ�.
	 *
	 * @param menuState ������ �޴� ����
	 */
	void setMenuState(int menuState);
	/**
	 * @brief �÷��̾��� ���� Ŭ������ ���������� �����ɴϴ�.
	 *
	 * �÷��̾��� ���� Ŭ������ ���������� ��ȯ�մϴ�.
	 *
	 * @param id �÷��̾��� ID
	 * @return int �÷��̾��� ���� Ŭ������ ��������
	 */
	int getPlayerCurrentClearStage(int id);
	/**
	 * @brief �÷��̾��� Ŭ������ ���������� �����մϴ�.
	 *
	 * �÷��̾��� Ŭ������ ���������� �����մϴ�.
	 *
	 * @param id �÷��̾��� ID
	 * @param stage ������ Ŭ������ ��������
	 */
	void setPlayerCurrentClearStage(int id, int stage);
	/**
	 * @brief �÷��̾��� �������� ������ �����ɴϴ�.
	 *
	 * �÷��̾��� �������� ������ ��ȯ�մϴ�.
	 *
	 * @param id �÷��̾��� ID
	 * @param stage ��������
	 * @return float �÷��̾��� �������� ����
	 */
	float getPlayerStageScore(int id, int stage);
	/**
	 * @brief �÷��̾��� �������� ������ �����մϴ�.
	 *
	 * �÷��̾��� �������� ������ �����մϴ�.
	 *
	 * @param id �÷��̾��� ID
	 * @param stage ��������
	 * @param score ������ �������� ����
	 */
	void setPlayerStageScore(int id, int stage, float score);
	/**
	 * @brief �������� ���� ���θ� Ȯ���մϴ�.
	 *
	 * �������� ���� ���θ� ��ȯ�մϴ�.
	 *
	 * @return bool �������� ���� ����
	 */
	bool isServerConnected();
	/**
	 * @brief Ŭ���̾�Ʈ ��ü�� �����ɴϴ�.
	 *
	 * Ŭ���̾�Ʈ ��ü�� ��ȯ�մϴ�.
	 *
	 * @return Client* Ŭ���̾�Ʈ ��ü
	 */
	Client* getClient();
	/**
	 * @brief IO ���ؽ�Ʈ�� �����ɴϴ�.
	 *
	 * IO ���ؽ�Ʈ�� ��ȯ�մϴ�.
	 *
	 * @return io_context& IO ���ؽ�Ʈ
	 */
	io_context& getIoContext();
	/**
	 * @brief ����� UID�� �����ɴϴ�.
	 *
	 * ����� UID�� ��ȯ�մϴ�.
	 *
	 * @return int ����� UID
	 */
	int getUid() const;
	/**
	 * @brief ����� UID�� �����մϴ�.
	 *
	 * ����� UID�� �����մϴ�.
	 *
	 * @param ������ ����� UID
	 */
	void setUid(int uid);
	/**
	 * @brief ����� �̸��� �����ɴϴ�.
	 *
	 * ����� �̸��� ��ȯ�մϴ�.
	 *
	 * @return string ����� �̸�
	 */
	string getUsername();
	/**
	 * @brief �� Ǯ ��ü�� �����ɴϴ�.
	 *
	 * �� Ǯ ��ü�� ��ȯ�մϴ�.
	 *
	 * @return EnemyPool* �� Ǯ ��ü
	 */
	EnemyPool* GetEnemyPool();

	Vector2f _playerPosition; // �÷��̾� ��ġ
	int _savePositionX = PLAYER_NO_SAVE_POSITION; // �÷��̾� ���̺� ����Ʈ ��ġ X
	int _savePositionY = PLAYER_NO_SAVE_POSITION; // �÷��̾� ���̺� ����Ʈ ��ġ Y

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