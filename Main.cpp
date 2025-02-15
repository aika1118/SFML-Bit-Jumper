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
	RenderWindow window(VideoMode(1366, 768), "Project W", Style::Titlebar | Style::Close); // 마우스 드래그로 창 크기 조절 불가
	Clock deltaClock;
	Game &game = Game::getInstance();
	Renderer renderer(window); // 렌더링될 타겟을 window로 지정

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

			// 점프 처리도 event를 통해 진행하면 깔끔하게 구현이 되지 않을까 생각
		}

		window.clear();

		game.Update(deltaTime, window);
		game.Render(renderer);
		game.RenderUI(renderer);

		window.display();
	}
}