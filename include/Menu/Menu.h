#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.h"

using namespace sf;

class Menu {
public:
    virtual ~Menu() = default; // ���� �Ҹ���
	/**
	 *
	 * �� �Լ��� �޴��� �ʱ� ������ ���� ���˴ϴ�.
	 *
	 * @param window �޴��� �ʱ�ȭ�� â�Դϴ�.
	 */
    virtual void init(RenderWindow& window) = 0;
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
    virtual void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) = 0; 
	/**
	 * �޴��� �������մϴ�.
	 *
	 * �� �Լ��� �޴��� ȭ�鿡 �׸��� ���� ���˴ϴ�.
	 *
	 * @param renderer �޴��� �׸��� ���� ����� �������Դϴ�.
	 */
    virtual void render(Renderer& renderer) = 0; 
};