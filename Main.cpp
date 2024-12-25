#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Game.h"
#include "Renderer.h"

int main()
{
	RenderWindow window(VideoMode(1366, 768), "Project W", Style::Titlebar | Style::Close); // 마우스 드래그로 창 크기 조절 불가
	Clock deltaClock;
	Game &game = Game::getInstance();
	Renderer renderer(window); // 렌더링될 타겟을 window로 지정

	// window.setFramerateLimit(30);

	game.Begin(window);
	
	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		Event event;
		 
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();


		game.Update(deltaTime);
		game.Render(renderer);
		game.RenderUI(renderer);

		window.display();
	}
}