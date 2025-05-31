#pragma once

#include "Menu.h"
#include "Config.h"
#include <iostream>

using namespace std;

class MainMenu : public Menu
{
public:
	/**
	 * @brief 메인 메뉴를 초기화합니다.
	 *
	 * 메인 메뉴를 초기화하고 필요한 리소스를 로드합니다.
	 *
	 * @param window 렌더링에 사용할 RenderWindow 객체
	 */
	void init(RenderWindow& window) override;
	/**
	 * @brief 메인 메뉴를 업데이트 합니다.
	 *
	 * 메인 메뉴의 상태를 업데이트하고 이벤트를 처리합니다.
	 *
	 * @param window 렌더링에 사용할 RenderWindow 객체
	 * @param event 현재 이벤트 객체
	 * @param deltaTime 이전 프레임 이후 경과된 시간
	 * @param nextState 다음 메뉴 상태
	 */
	void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	/**
	 * @brief 메인 메뉴를 렌더링합니다.
	 *
	 * 메인 메뉴를 렌더링하고 화면에 표시합니다.
	 *
	 * @param renderer 렌더링에 사용할 Renderer 객체
	 */
	void render(Renderer& renderer) override;

private:
	Font _font;
	Text _playText;
	Text _exitText;
	Text _rankingText;
};