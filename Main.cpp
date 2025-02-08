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
	// window.setFramerateLimit(30);

	/*******************Ŭ���̾�Ʈ ���� �׽�Ʈ �ڵ� ����*****************/
	/*
	try
	{
		io_context io_context; // I/O ���ؽ�Ʈ ����
		string host = "127.0.0.1"; // ����(IP)�� ����
		string port = "12345"; // ���� ��Ʈ�� ����
		Client client(io_context, host, port); // ������ ����
		string id = "4";
		string data = "test_plan_name,5";
		client.send_packet_async(PACKET_READ, id); // READ_PACKET ���� (�񵿱������� ����)
		//client.send_packet_async(PACKET_WRITE, data); // WRITE_PACKET ���� (�񵿱������� ����)
		// �ʿ� �� PACKET_SAVE ��Ŷ ���� 

		// I/O ���ؽ�Ʈ ����
		io_context.run();
	}
	catch (const std::exception& e)
	{
		cerr << "Exception: " << e.what() << "\n"; // ����ó��
	}
	*/
	/*******************Ŭ���̾�Ʈ ���� �׽�Ʈ �ڵ� ����************************/

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