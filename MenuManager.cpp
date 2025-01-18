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
    menus.push_back(make_unique<MainMenu>());
    menus.push_back(make_unique<StageMenu>());

    for (const auto& menu : menus) 
    {
        if (!menu) continue;
        menu->init(window);
    }
}

void MenuManager::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    setMenu(nextState);

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

void MenuManager::setMenu(int menuIndex)
{
    currentMenu = menuIndex;
}

bool MenuManager::isInMenu()
{   
    return currentMenu >= 0 && currentMenu < menus.size();
}

