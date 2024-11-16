// File: Game.h
// Purpose: ���� �ʱ�ȭ, ������Ʈ, ������ ����

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
	static Game& getInstance(); // �̱���

	void Begin(const RenderWindow& window); // ���� ���� �� �ʱ�ȭ �۾��� ����
	void Update(float deltaTime); // ���� ������Ʈ �۾�
	void Render(Renderer& renderer); // ���� ������ �۾�

private:
	Game(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	Game(const Game&) = delete; // ��������� ����
	Game& operator=(const Game&) = delete; // ���Կ����� ����

	Camera& camera;
	Map& gameMap;
	Image _mapImage; // ���� ���ȭ��
	vector<Object*> _objects; // ���� �� object ����
};