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
    * �������� �޴� �ʱ�ȭ �Լ��Դϴ�.
    * �������� �޴��� �ʱ� ���¸� �����մϴ�.
    * @param window ������ ������
    */
    void init(RenderWindow& window) override;
    /**
    * �������� �޴� ������Ʈ �Լ��Դϴ�.
    * �������� �޴��� ���¸� ������Ʈ �մϴ�.
    * @param window ������ ������
    * @param event �̺�Ʈ ��ü
    * @param deltaTime �ð� ����
    * @param nextState ���� ����
    */
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
    /**
    * �������� �޴� ������ �Լ��Դϴ�.
    * �������� �޴��� ȭ�鿡 �׸��� ���� ȣ��˴ϴ�.
    * @param renderer ������ ��ü
    */
    void render(Renderer& renderer) override;

private:
    Font _font; // StageMenu�� ����� ��Ʈ
    vector<Text> stageTexts; // stage ǥ�� text
    Text _returnPageText; // "return" ǥ�ø� ���� text
    Text _loadingText; // "loading" ǥ�ø� ���� text
};