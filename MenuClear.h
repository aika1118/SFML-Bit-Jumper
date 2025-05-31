#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>

using namespace std;

class MenuClear : public Menu {
public:
	/**
	* 메뉴 초기화 함수
	* @param window 렌더링 윈도우
	*/
    void init(RenderWindow& window) override;
	/**
	* 메뉴 업데이트 함수
	* @param window 렌더링 윈도우
	* @param event 이벤트 객체
	* @param deltaTime 시간 차이
	* @param nextState 다음 상태
	*/
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	/**
	* 메뉴 렌더링 함수
	* @param renderer 렌더러 객체
	*/
    void render(Renderer& renderer) override;

private:
	Font _font;
	Text _clearText;
	Text _scoreText;
	Text _okText;
	Text _loadingText;
};