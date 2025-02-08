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
	// window.setFramerateLimit(30);

	/*******************클라이언트 연결 테스트 코드 시작*****************/
	/*
	try
	{
		io_context io_context; // I/O 컨텍스트 생성
		string host = "127.0.0.1"; // 서버(IP)를 정의
		string port = "12345"; // 서버 포트를 정의
		Client client(io_context, host, port); // 서버에 연결
		string id = "4";
		string data = "test_plan_name,5";
		client.send_packet_async(PACKET_READ, id); // READ_PACKET 전송 (비동기적으로 실행)
		//client.send_packet_async(PACKET_WRITE, data); // WRITE_PACKET 전송 (비동기적으로 실행)
		// 필요 시 PACKET_SAVE 패킷 전송 

		// I/O 컨텍스트 실행
		io_context.run();
	}
	catch (const std::exception& e)
	{
		cerr << "Exception: " << e.what() << "\n"; // 예외처리
	}
	*/
	/*******************클라이언트 연결 테스트 코드 종료************************/

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

			// 점프 처리도 event를 통해 진행하면 깔끔하게 구현이 되지 않을까 생각
		}

		window.clear();

		game.Update(deltaTime, window);
		game.Render(renderer);
		game.RenderUI(renderer);

		window.display();
	}
}