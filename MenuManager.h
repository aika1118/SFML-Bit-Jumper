#pragma once

#include "MainMenu.h"
#include "StageMenu.h"
#include "MenuClear.h"
#include "MenuCreateUserName.h"
#include <memory>
#include <vector>
#include <iostream>

class MenuManager {
public:
    static MenuManager& getInstance();
    void init(RenderWindow& window);
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState);
    void render(Renderer& renderer);
    void setCurrentMenu(int menuIndex);
    bool isInMenu();
    Menu* getMenu(int menuIndex);

private:
    MenuManager();
    MenuManager(const MenuManager&) = delete;
    MenuManager& operator=(const MenuManager&) = delete;

    vector<unique_ptr<Menu>> menus;
    int currentMenu;
};