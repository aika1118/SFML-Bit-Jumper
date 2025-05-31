#pragma once

#include "Menu.h"
#include "Config.h"
#include <iostream>

using namespace std;

class MainMenu : public Menu
{
public:
	/**
	 * @brief ���� �޴��� �ʱ�ȭ�մϴ�.
	 *
	 * ���� �޴��� �ʱ�ȭ�ϰ� �ʿ��� ���ҽ��� �ε��մϴ�.
	 *
	 * @param window �������� ����� RenderWindow ��ü
	 */
	void init(RenderWindow& window) override;
	/**
	 * @brief ���� �޴��� ������Ʈ �մϴ�.
	 *
	 * ���� �޴��� ���¸� ������Ʈ�ϰ� �̺�Ʈ�� ó���մϴ�.
	 *
	 * @param window �������� ����� RenderWindow ��ü
	 * @param event ���� �̺�Ʈ ��ü
	 * @param deltaTime ���� ������ ���� ����� �ð�
	 * @param nextState ���� �޴� ����
	 */
	void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	/**
	 * @brief ���� �޴��� �������մϴ�.
	 *
	 * ���� �޴��� �������ϰ� ȭ�鿡 ǥ���մϴ�.
	 *
	 * @param renderer �������� ����� Renderer ��ü
	 */
	void render(Renderer& renderer) override;

private:
	Font _font;
	Text _playText;
	Text _exitText;
	Text _rankingText;
};