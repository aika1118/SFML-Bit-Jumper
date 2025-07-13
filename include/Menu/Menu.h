#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.h"

using namespace sf;

class Menu {
public:
    virtual ~Menu() = default; // 가상 소멸자
	/**
	 *
	 * 이 함수는 메뉴의 초기 설정을 위해 사용됩니다.
	 *
	 * @param window 메뉴를 초기화할 창입니다.
	 */
    virtual void init(RenderWindow& window) = 0;
	/**
	 * 메뉴를 업데이트 합니다.
	 *
	 * 이 함수는 메뉴의 상태를 업데이트 하기 위해 사용됩니다.
	 *
	 * @param window 메뉴를 업데이트 할 창입니다.
	 * @param event 메뉴를 업데이트 할 이벤트입니다.
	 * @param deltaTime 시간차입니다.
	 * @param nextState 다음 상태입니다.
	 */
    virtual void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) = 0; 
	/**
	 * 메뉴를 렌더링합니다.
	 *
	 * 이 함수는 메뉴를 화면에 그리기 위해 사용됩니다.
	 *
	 * @param renderer 메뉴를 그리기 위해 사용할 렌더러입니다.
	 */
    virtual void render(Renderer& renderer) = 0; 
};