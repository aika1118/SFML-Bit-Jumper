#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Game.h"
#include "Renderer.h"
#include "MenuManager.h"
#include <thread>
#include <mysql/jdbc.h>

/************���� ���� �׽�Ʈ �Լ� ����**************/
void checkServerConnection(const std::string& serverIP, unsigned short port) {
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(serverIP, port, sf::seconds(5)); // 5�� Ÿ�Ӿƿ�

	if (status == sf::Socket::Done) {
		std::cout << "Successfully connected to the server!" << std::endl;
	}
	else {
		std::cout << "Failed to connect to the server." << std::endl;
	}
}
/************���� ���� �׽�Ʈ �Լ� ����**************/

int main()
{
	RenderWindow window(VideoMode(1366, 768), "Project W", Style::Titlebar | Style::Close); // ���콺 �巡�׷� â ũ�� ���� �Ұ�
	Clock deltaClock;
	Game &game = Game::getInstance();
	Renderer renderer(window); // �������� Ÿ���� window�� ����
	// window.setFramerateLimit(30);

	/************���� ���� �׽�Ʈ �ڵ� ����**************/

	//std::string serverIP = "35.247.65.229";  // ���� IP
	//unsigned short port = 22;          // ���� ��Ʈ

	//// ��Ƽ�������� ����Ͽ� ���� ���� üũ
	//std::thread connectionThread(checkServerConnection, serverIP, port);
	//connectionThread.join(); // ������ ������� ���

	/************���� ���� �׽�Ʈ �ڵ� ����**************/

	/********************MySQL ���� �׽�Ʈ �ڵ� ����*****************/
	/*try {
		sql::Driver* driver = get_driver_instance();
		std::unique_ptr<sql::Connection> con(driver->connect("tcp://35.247.65.229:3306/trip_plan", "wskang", "Angelic3545!@"));

		if (con->isValid()) {
			std::cout << "�����ͺ��̽��� ���������� ����Ǿ����ϴ�." << std::endl;
		}
	}
	catch (sql::SQLException& e) {
		std::cout << "���� ����: " << e.what() << std::endl;
	}*/
	/*******************MySQL ���� �׽�Ʈ �ڵ� ����************************/

	game.Begin(window);
	
	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		Event event;
		 
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (MenuManager::getInstance().isInMenu())
				MenuManager::getInstance().update(window, event, deltaTime, Game::getInstance().getMenuState());

			// ���� ó���� event�� ���� �����ϸ� ����ϰ� ������ ���� ������ ����
		}

		window.clear();

		game.Update(deltaTime, window);
		game.Render(renderer);
		game.RenderUI(renderer);

		window.display();
	}
}