#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Game.h"
#include "Renderer.h"
#include "MenuManager.h"
#include <thread>
#include <mysql/jdbc.h>
#include "Client.h"
#include <boost/asio.hpp>
#include "Config.h"

int main()
{
	RenderWindow window(VideoMode(1366, 768), "Project W", Style::Titlebar | Style::Close); // ���콺 �巡�׷� â ũ�� ���� �Ұ�
	Clock deltaClock;
	Game &game = Game::getInstance();
	Renderer renderer(window); // �������� Ÿ���� window�� ����

	game.Begin(window);
	
	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		Event event;
		 
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (MenuManager::getInstance().isInMenu())
			{	
				//cout << "Menu State : " << Game::getInstance().getMenuState() << endl;
				MenuManager::getInstance().update(window, event, deltaTime, Game::getInstance().getMenuState());
			}

			// ���� ó���� event�� ���� �����ϸ� ����ϰ� ������ ���� ������ ����
		}

		window.clear();

		game.Update(deltaTime, window);
		game.Render(renderer);
		game.RenderUI(renderer);

		window.display();
	}
}