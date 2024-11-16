#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Game.h"
#include "Renderer.h"

int main()
{
	RenderWindow window(VideoMode(1366, 768), "Project W");
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
				window.close();

			//if (event.type == Event::Resized) // 창 크기 변할 때 View 재조정
			//{
			//	FloatRect visibleArea(0.f, 0.f, (float)event.size.width, (float)event.size.height);
			//	window.setView(View(window.getView().getCenter(), sf::Vector2f((float)event.size.width, (float)event.size.height)));
			//}
		}

		window.clear();


		game.Update(deltaTime);
		game.Render(renderer);

		window.display();
	}
}