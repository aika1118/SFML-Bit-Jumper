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
#include "Mushroom.h"


using namespace sf;

class Map
{
public:
	/**
	 * 맵 클래스의 싱글턴 인스턴스를 반환합니다.
	 *
	 * 이 함수는 맵 클래스의 싱글턴 인스턴스에 접근하기 위해 사용됩니다.
	 *
	 * @return 맵 클래스의 싱글턴 인스턴스입니다.
	 */
	static Map& getInstance(); 
	/**
	 * 이미지로부터 맵을 생성합니다.
	 *
	 * 이 함수는 제공된 이미지와 객체를 기반으로 맵을 생성합니다.
	 *
	 * @param image 맵의 기반이 되는 이미지입니다.
	 * @param objects 맵에 포함할 객체입니다.
	 */
	void CreateFromImage(const Image& image, vector<Object*>& objects);
	/**
	 * 렌더러를 사용하여 맵을 그립니다.
	 *
	 * 이 함수는 맵을 화면에 그리기 위해 사용됩니다.
	 *
	 * @param renderer 맵을 그리기 위해 사용할 렌더러입니다.
	 */
	void Draw(Renderer& renderer);
	/**
	 * 지정된 스테이지의 맵 이미지들을 반환합니다.
	 *
	 * 이 함수는 지정된 스테이지와 관련된 맵 이미지들을 반환합니다.
	 *
	 * @param stage 맵 이미지들을 가져올 스테이지입니다.
	 * @return 지정된 스테이지의 맵 이미지들입니다.
	 */
	string getMapImages(int stage);

	vector<vector<Texture*>> _grid; // 그리드에 그려질 texture 관리
	float _cellSize = MAP_CELL_SIZE; // 그리드를 구성하는 셀 사이즈

	unordered_map<string, vector<b2Body*>> bodies; // b2body 종류와 실제 body들을 저장하는 자료구조 (데이터 보호를 위해 private으로 뺴야하지 않을까?), 현재 lock 블록만 처리중 (나머지도 추가필요)
	unordered_map<string, vector<pair<int, int>>> textures; // texture 종류와 해당 위치를 저장하는 자료구조, 현재 lock 블록만 처리중 (나머지도 추가필요)

private:
	Map(); // 외부에서 생성자 호출 불가하도록 설정
	Map(const Map&) = delete; // 복사생성자 삭제
	Map& operator=(const Map&) = delete; // 대입연산자 삭제
	
	unordered_map<int, string> mapImages;
	
};