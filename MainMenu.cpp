#include "MainMenu.h"
#include "Game.h"

void MainMenu::init(RenderWindow& window)
{
    if (!_font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
    {
        cout << "No font is here" << endl;
        return;
    }

    // Play
    _playText.setFont(_font);
    _playText.setFillColor(Color::White);
    _playText.setString("Play");
    _playText.setCharacterSize(60);
    _playText.setOrigin(_playText.getLocalBounds().width / 2, _playText.getLocalBounds().height / 2);
    _playText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 - 90);


    // Ranking
    _rankingText.setFont(_font);
    _rankingText.setFillColor(Color::White);
    if (Game::getInstance().isServerConnected() == false) _rankingText.setFillColor(Color(100, 100, 100)); // 오프라인 모드인 경우 Ranking 음영처리
    _rankingText.setString("Ranking");
    _rankingText.setCharacterSize(60);
    _rankingText.setOrigin(_rankingText.getLocalBounds().width / 2, _rankingText.getLocalBounds().height / 2);
    _rankingText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 30);

	// Exit
	_exitText.setFont(_font);
	_exitText.setFillColor(Color::White);
	_exitText.setString("Exit");
	_exitText.setCharacterSize(60);
    _exitText.setOrigin(_exitText.getLocalBounds().width / 2, _exitText.getLocalBounds().height / 2);
	_exitText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 150);
}

void MainMenu::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (_playText.getGlobalBounds().contains(mousePos))
        { 
            nextState = MenuIndex::STAGE_MENU; // StageMenu로 이동
        }
        else if (_exitText.getGlobalBounds().contains(mousePos))
            nextState = MenuIndex::EXIT; // 게임 종료
        else if (_rankingText.getGlobalBounds().contains(mousePos))
        {
            if (Game::getInstance().isServerConnected() == false) return; // offline 모드인 경우 랭킹 진입을 허용하지 않음

            nextState = MenuIndex::RANKING_MENU; // 랭킹 메뉴
            Game::getInstance().getClient()->send_packet_async(PACKET_READ_RANKING, "",
                [](const string& response) { // 콜백 정의
                    cout << "PACKET_READ_RANKING callback!" << endl;
                    MenuRanking* menuRanking = dynamic_cast<MenuRanking*>(MenuManager::getInstance().getMenu(MenuIndex::RANKING_MENU)); // 안전하게 다운캐스팅 (해당 형식이 아닐경우 nullptr 반환)
                    if (!menuRanking)
                    {
                        cout << "menuRanking is null!" << endl;
                        return;
                    }

                    // 한줄씩 데이터를 읽어서 username, score 데이터를 parameter로 넘기기 
                    stringstream ss(response);
                    string line;

                    while (getline(ss, line))
                    {
                        stringstream lineStream(line);
                        string name;
                        string score;

                        // space로 구분된 name, score 읽은 후 rankData에 삽입
                        if (lineStream >> name >> score)
                            menuRanking->pushRankData(name, score);
                    }

                    return;
                }
            );
        }
    }
}

void MainMenu::render(Renderer& renderer)
{
    renderer._target.draw(_playText);
    renderer._target.draw(_exitText);
    renderer._target.draw(_rankingText);
}
