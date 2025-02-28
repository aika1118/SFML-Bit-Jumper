#include "MenuManager.h"


MenuManager::MenuManager()
    : currentMenu(MenuIndex::MAIN_MENU)
{
}

MenuManager& MenuManager::getInstance()
{
	static MenuManager instance;
	return instance;
}

void MenuManager::init(RenderWindow& window)
{
    // 새로운 매뉴 추가 후 push_back 필요
    menus.push_back(make_unique<MainMenu>());
    menus.push_back(make_unique<StageMenu>());
    menus.push_back(make_unique<MenuClear>());
    menus.push_back(make_unique<MenuCreateUserName>());
    menus.push_back(make_unique<MenuRanking>());

    for (const auto& menu : menus) 
    {
        if (!menu) continue;
        menu->init(window);
    }
}

void MenuManager::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    setCurrentMenu(nextState);

    if (currentMenu >= 0 && currentMenu < menus.size()) 
    {
        menus[currentMenu]->update(window, event, deltaTime, nextState);
    }
}

void MenuManager::render(Renderer& renderer)
{
    if (currentMenu >= 0 && currentMenu < menus.size()) 
	{
		menus[currentMenu]->render(renderer);
	}
}

void MenuManager::setCurrentMenu(int menuIndex)
{
    currentMenu = menuIndex;
}

bool MenuManager::isInMenu()
{   
    return currentMenu >= 0 && currentMenu < menus.size();
}

Menu* MenuManager::getMenu(int menuIndex)
{
    if (menuIndex >= 0 && menuIndex < menus.size()) return menus[menuIndex].get(); 
    
    return nullptr;
}

