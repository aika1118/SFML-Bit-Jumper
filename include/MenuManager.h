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
     * �޴� �Ŵ��� Ŭ������ �̱��� �ν��Ͻ��� ��ȯ�մϴ�.
     *
     * �� �Լ��� �޴� �Ŵ��� Ŭ������ �̱��� �ν��Ͻ��� �����ϱ� ���� ���˴ϴ�.
     *
     * @return �޴� �Ŵ��� Ŭ������ �̱��� �ν��Ͻ��Դϴ�.
     */
    static MenuManager& getInstance();
    /**
     * �޴��� �ʱ�ȭ�մϴ�.
     *
     * �� �Լ��� �޴��� �ʱ� ������ ���� ���˴ϴ�.
     *
     * @param window �޴��� �ʱ�ȭ�� â�Դϴ�.
     */
    void init(RenderWindow& window);
    /**
     * �޴��� ������Ʈ �մϴ�.
     *
     * �� �Լ��� �޴��� ���¸� ������Ʈ �ϱ� ���� ���˴ϴ�.
     *
     * @param window �޴��� ������Ʈ �� â�Դϴ�.
     * @param event �޴��� ������Ʈ �� �̺�Ʈ�Դϴ�.
     * @param deltaTime �ð����Դϴ�.
     * @param nextState ���� �����Դϴ�.
     */
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState);
    /**
     * �޴��� �������մϴ�.
     *
     * �� �Լ��� �޴��� ȭ�鿡 �׸��� ���� ���˴ϴ�.
     *
     * @param renderer �޴��� �׸��� ���� ����� �������Դϴ�.
     */
    void render(Renderer& renderer);
    /**
     * ���� �޴��� �����մϴ�.
     *
     * �� �Լ��� ���� �޴��� �����ϱ� ���� ���˴ϴ�.
     *
     * @param menuIndex ���� �޴��� �ε����Դϴ�.
     */
    void setCurrentMenu(int menuIndex);
    /**
     *
     * �� �Լ��� ���� �޴��� �ִ��� ���θ� ��ȯ�ϱ� ���� ���˴ϴ�.
     *
     * @return ���� �޴��� �ִ��� �����Դϴ�.
     */
    bool isInMenu();
    /**
     * ������ �ε����� �޴��� ��ȯ�մϴ�.
     *
     * �� �Լ��� ������ �ε����� �޴��� ��ȯ�ϱ� ���� ���˴ϴ�.
     *
     * @param menuIndex �޴��� �ε����Դϴ�.
     * @return ������ �ε����� �޴��Դϴ�.
     */
    Menu* getMenu(int menuIndex);

private:
    MenuManager();
    MenuManager(const MenuManager&) = delete;
    MenuManager& operator=(const MenuManager&) = delete;

    vector<unique_ptr<Menu>> menus; // ���� ���� �޴� ������
    int currentMenu; // ���� �޴� index
};