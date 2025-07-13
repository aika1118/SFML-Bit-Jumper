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
#include "BoxFragile.h"
#include "Mushroom.h"


using namespace sf;

class Map
{
public:
	/**
	 * �� Ŭ������ �̱��� �ν��Ͻ��� ��ȯ�մϴ�.
	 *
	 * �� �Լ��� �� Ŭ������ �̱��� �ν��Ͻ��� �����ϱ� ���� ���˴ϴ�.
	 *
	 * @return �� Ŭ������ �̱��� �ν��Ͻ��Դϴ�.
	 */
	static Map& getInstance(); 
	/**
	 * �̹����κ��� ���� �����մϴ�.
	 *
	 * �� �Լ��� ������ �̹����� ��ü�� ������� ���� �����մϴ�.
	 *
	 * @param image ���� ����� �Ǵ� �̹����Դϴ�.
	 * @param objects �ʿ� ������ ��ü�Դϴ�.
	 */
	void CreateFromImage(const Image& image, vector<Object*>& objects);
	/**
	 * �������� ����Ͽ� ���� �׸��ϴ�.
	 *
	 * �� �Լ��� ���� ȭ�鿡 �׸��� ���� ���˴ϴ�.
	 *
	 * @param renderer ���� �׸��� ���� ����� �������Դϴ�.
	 */
	void Draw(Renderer& renderer);
	/**
	 * ������ ���������� �� �̹������� ��ȯ�մϴ�.
	 *
	 * �� �Լ��� ������ ���������� ���õ� �� �̹������� ��ȯ�մϴ�.
	 *
	 * @param stage �� �̹������� ������ ���������Դϴ�.
	 * @return ������ ���������� �� �̹������Դϴ�.
	 */
	string getMapImages(int stage);

	vector<vector<Texture*>> _grid; // �׸��忡 �׷��� texture ����
	float _cellSize = MAP_CELL_SIZE; // �׸��带 �����ϴ� �� ������

	unordered_map<string, vector<b2Body*>> bodies; // b2body ������ ���� body���� �����ϴ� �ڷᱸ�� (������ ��ȣ�� ���� private���� �������� ������?), ���� lock ��ϸ� ó���� (�������� �߰��ʿ�)
	unordered_map<string, vector<pair<int, int>>> textures; // texture ������ �ش� ��ġ�� �����ϴ� �ڷᱸ��, ���� lock ��ϸ� ó���� (�������� �߰��ʿ�)

private:
	Map(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	Map(const Map&) = delete; // ��������� ����
	Map& operator=(const Map&) = delete; // ���Կ����� ����
	
	unordered_map<int, string> mapImages;
	
};