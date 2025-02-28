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

	// Exit
	_exitText.setFont(_font);
	_exitText.setFillColor(Color::White);
	_exitText.setString("Exit");
	_exitText.setCharacterSize(60);
    _exitText.setOrigin(_exitText.getLocalBounds().width / 2, _exitText.getLocalBounds().height / 2);
	_exitText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 30);

    // Ranking
    _rankingText.setFont(_font);
    _rankingText.setFillColor(Color::White);
    _rankingText.setString("Ranking");
    _rankingText.setCharacterSize(60);
    _rankingText.setOrigin(_rankingText.getLocalBounds().width / 2, _rankingText.getLocalBounds().height / 2);
    _rankingText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 150);
}

void MainMenu::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (_playText.getGlobalBounds().contains(mousePos))
            nextState = MenuIndex::STAGE_MENU; // StageMenu로 이동
        else if (_exitText.getGlobalBounds().contains(mousePos))
            nextState = MenuIndex::EXIT; // 게임 종료
        else if (_rankingText.getGlobalBounds().contains(mousePos))
        {
            nextState = MenuIndex::RANKING_MENU; // 랭킹 메뉴

            // 서버에서 랭킹 데이터 받아오기
            if (Util::isServerConnected()) 
                Game::getInstance().getClient()->send_packet_async(PACKET_READ_RANKING, "");
        }
    }
}

void MainMenu::render(Renderer& renderer)
{
    renderer._target.draw(_playText);
    renderer._target.draw(_exitText);
    renderer._target.draw(_rankingText);
}
