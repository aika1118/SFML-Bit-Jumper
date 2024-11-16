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
	Renderer renderer(window); // �������� Ÿ���� window�� ����

	game.Begin(window);
	
	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		Event event;
		 
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//if (event.type == Event::Resized) // â ũ�� ���� �� View ������
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