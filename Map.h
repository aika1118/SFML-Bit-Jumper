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


using namespace sf;

class Map
{
public:
	static Map& getInstance(); // 싱글톤

	Vector2f CreateFromImage(const Image& image, vector<Object*>& objects); // 도트 이미지 기반으로 맵 생성 (libresprite 툴 사용)

	void Draw(Renderer& renderer);

	vector<vector<Texture*>> _grid; // 그리드에 그려질 texture 관리
	float _cellSize = MAP_CELL_SIZE; // 그리드를 구성하는 셀 사이즈

private:
	Map(); // 외부에서 생성자 호출 불가하도록 설정
	Map(const Map&) = delete; // 복사생성자 삭제
	Map& operator=(const Map&) = delete; // 대입연산자 삭제
};