#pragma once

#include "MainMenu.h"
#include "StageMenu.h"
#include "MenuClear.h"
#include "MenuCreateUserName.h"
#include "MenuRanking.h"
#include <memory>
#include <vector>
#include <iostream>

class MenuManager {
public:
    /**
     * 메뉴 매니저 클래스의 싱글톤 인스턴스를 반환합니다.
     *
     * 이 함수는 메뉴 매니저 클래스의 싱글톤 인스턴스에 접근하기 위해 사용됩니다.
     *
     * @return 메뉴 매니저 클래스의 싱글톤 인스턴스입니다.
     */
    static MenuManager& getInstance();
    /**
     * 메뉴를 초기화합니다.
     *
     * 이 함수는 메뉴의 초기 설정을 위해 사용됩니다.
     *
     * @param window 메뉴를 초기화할 창입니다.
     */
    void init(RenderWindow& window);
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
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState);
    /**
     * 메뉴를 렌더링합니다.
     *
     * 이 함수는 메뉴를 화면에 그리기 위해 사용됩니다.
     *
     * @param renderer 메뉴를 그리기 위해 사용할 렌더러입니다.
     */
    void render(Renderer& renderer);
    /**
     * 현재 메뉴를 설정합니다.
     *
     * 이 함수는 현재 메뉴를 설정하기 위해 사용됩니다.
     *
     * @param menuIndex 현재 메뉴의 인덱스입니다.
     */
    void setCurrentMenu(int menuIndex);
    /**
     *
     * 이 함수는 현재 메뉴에 있는지 여부를 반환하기 위해 사용됩니다.
     *
     * @return 현재 메뉴에 있는지 여부입니다.
     */
    bool isInMenu();
    /**
     * 지정된 인덱스의 메뉴를 반환합니다.
     *
     * 이 함수는 지정된 인덱스의 메뉴를 반환하기 위해 사용됩니다.
     *
     * @param menuIndex 메뉴의 인덱스입니다.
     * @return 지정된 인덱스의 메뉴입니다.
     */
    Menu* getMenu(int menuIndex);

private:
    MenuManager();
    MenuManager(const MenuManager&) = delete;
    MenuManager& operator=(const MenuManager&) = delete;

    vector<unique_ptr<Menu>> menus; // 관리 중인 메뉴 보관용
    int currentMenu; // 현재 메뉴 index
};