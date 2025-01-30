#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Game.h"
#include "Renderer.h"
#include "MenuManager.h"
#include <thread>
#include <mysql/jdbc.h>

/************서버 연결 테스트 함수 시작**************/
void checkServerConnection(const std::string& serverIP, unsigned short port) {
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(serverIP, port, sf::seconds(5)); // 5초 타임아웃

	if (status == sf::Socket::Done) {
		std::cout << "Successfully connected to the server!" << std::endl;
	}
	else {
		std::cout << "Failed to connect to the server." << std::endl;
	}
}
/************서버 연결 테스트 함수 종료**************/

int main()
{
	RenderWindow window(VideoMode(1366, 768), "Project W", Style::Titlebar | Style::Close); // 마우스 드래그로 창 크기 조절 불가
	Clock deltaClock;
	Game &game = Game::getInstance();
	Renderer renderer(window); // 렌더링될 타겟을 window로 지정
	// window.setFramerateLimit(30);

	/************서버 연결 테스트 코드 시작**************/

	//std::string serverIP = "35.247.65.229";  // 서버 IP
	//unsigned short port = 22;          // 서버 포트

	//// 멀티스레딩을 사용하여 서버 연결 체크
	//std::thread connectionThread(checkServerConnection, serverIP, port);
	//connectionThread.join(); // 스레드 종료까지 대기

	/************서버 연결 테스트 코드 종료**************/

	/********************MySQL 연결 테스트 코드 시작*****************/
	/*try {
		sql::Driver* driver = get_driver_instance();
		std::unique_ptr<sql::Connection> con(driver->connect("tcp://35.247.65.229:3306/trip_plan", "wskang", "Angelic3545!@"));

		if (con->isValid()) {
			std::cout << "데이터베이스에 성공적으로 연결되었습니다." << std::endl;
		}
	}
	catch (sql::SQLException& e) {
		std::cout << "연결 실패: " << e.what() << std::endl;
	}*/
	/*******************MySQL 연결 테스트 코드 종료************************/

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

			// 점프 처리도 event를 통해 진행하면 깔끔하게 구현이 되지 않을까 생각
		}

		window.clear();

		game.Update(deltaTime, window);
		game.Render(renderer);
		game.RenderUI(renderer);

		window.display();
	}
}