// File: Map.h
// Brief: �� ����

#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Renderer.h"
#include "Object.h"
#include "Resources.h"
#include "Config.h"
#include "physics.h"
#include "FixtureData.h"
#include "Coin.h"
#include "Enemy.h"
#include "Spike.h"


using namespace sf;

class Map
{
public:
	static Map& getInstance(); // �̱���

	void CreateFromImage(const Image& image, vector<Object*>& objects); // ��Ʈ �̹��� ������� �� ���� (libresprite �� ���)

	void Draw(Renderer& renderer);

	vector<vector<Texture*>> _grid; // �׸��忡 �׷��� texture ����
	float _cellSize = MAP_CELL_SIZE; // �׸��带 �����ϴ� �� ������

	string getMapImages(int stage);

private:
	Map(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	Map(const Map&) = delete; // ��������� ����
	Map& operator=(const Map&) = delete; // ���Կ����� ����
	
	unordered_map<int, string> mapImages;
};