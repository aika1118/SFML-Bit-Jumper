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
	int _savePositionX = 0;
	int _savePositionY = 0;

	void InitSkill();

	Vector2f getPlayerPosition(); // player�� ���� position ��ȯ
	float getPlayerAngle(); // player�� ���� angle ��ȯ
	bool getPlayerFacingLeft(); // player�� �ٶ󺸴� ���� ��ȯ
	Player& getPlayer(); 
	int& getMenuState();


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
	Text gameOverText;

	RectangleShape backgroundWhenPaused; // �Ͻ������� ���� ���

	FloatRect _mapBound;

	int _menuState = MenuIndex::MAIN_MENU;
};