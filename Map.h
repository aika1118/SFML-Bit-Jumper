// File: Map.h
// Brief: �� ����

#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Object.h"
#include "Resources.h"
#include "Config.h"

using namespace sf;

class Map
{
public:
	static Map& getInstance(); // �̱���

	Vector2f CreateFromImage(const Image& image, vector<Object*>& objects); // ��Ʈ �̹��� ������� �� ���� (libresprite �� ���)

	void Draw(Renderer& renderer);

	vector<vector<Texture*>> _grid; // �׸��忡 �׷��� texture ����
	float _cellSize = MAP_CELL_SIZE; // �׸��带 �����ϴ� �� ������

private:
	Map(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	Map(const Map&) = delete; // ��������� ����
	Map& operator=(const Map&) = delete; // ���Կ����� ����
};