#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>

using namespace std;

class StageMenu : public Menu {
public:
    /**
    * 스테이지 메뉴 초기화 함수입니다.
    * 스테이지 메뉴의 초기 상태를 설정합니다.
    * @param window 렌더링 윈도우
    */
    void init(RenderWindow& window) override;
    /**
    * 스테이지 메뉴 업데이트 함수입니다.
    * 스테이지 메뉴의 상태를 업데이트 합니다.
    * @param window 렌더링 윈도우
    * @param event 이벤트 객체
    * @param deltaTime 시간 차이
    * @param nextState 다음 상태
    */
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
    /**
    * 스테이지 메뉴 렌더링 함수입니다.
    * 스테이지 메뉴를 화면에 그리기 위해 호출됩니다.
    * @param renderer 렌더러 객체
    */
    void render(Renderer& renderer) override;

private:
    Font _font; // StageMenu에 사용할 폰트
    vector<Text> stageTexts; // stage 표시 text
    Text _returnPageText; // "return" 표시를 위한 text
    Text _loadingText; // "loading" 표시를 위한 text
};