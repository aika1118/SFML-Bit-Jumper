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
	* �޴� �ʱ�ȭ �Լ�
	* @param window ������ ������
	*/
    void init(RenderWindow& window) override;
	/**
	* �޴� ������Ʈ �Լ�
	* @param window ������ ������
	* @param event �̺�Ʈ ��ü
	* @param deltaTime �ð� ����
	* @param nextState ���� ����
	*/
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	/**
	* �޴� ������ �Լ�
	* @param renderer ������ ��ü
	*/
    void render(Renderer& renderer) override;

private:
	Font _font;
	Text _clearText;
	Text _scoreText;
	Text _okText;
	Text _loadingText;
};