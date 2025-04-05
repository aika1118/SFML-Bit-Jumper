// File: Map.h
// Brief: 맵 관리

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


using namespace sf;

class Map
{
public:
	static Map& getInstance(); // 싱글톤

	void CreateFromImage(const Image& image, vector<Object*>& objects); // 도트 이미지 기반으로 맵 생성 (libresprite 툴 사용)

	void Draw(Renderer& renderer);

	vector<vector<Texture*>> _grid; // 그리드에 그려질 texture 관리
	float _cellSize = MAP_CELL_SIZE; // 그리드를 구성하는 셀 사이즈

	string getMapImages(int stage);

	unordered_map<string, vector<b2Body*>> bodies; // b2body 종류와 실제 body들을 저장하는 자료구조 (데이터 보호를 위해 private으로 뺴야하지 않을까?), 현재 lock 블록만 처리중 (나머지도 추가필요)
	unordered_map<string, vector<pair<int, int>>> textures; // texture 종류와 해당 위치를 저장하는 자료구조, 현재 lock 블록만 처리중 (나머지도 추가필요)

private:
	Map(); // 외부에서 생성자 호출 불가하도록 설정
	Map(const Map&) = delete; // 복사생성자 삭제
	Map& operator=(const Map&) = delete; // 대입연산자 삭제
	
	unordered_map<int, string> mapImages;
	
};