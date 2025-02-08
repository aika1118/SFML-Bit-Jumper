#include "MenuClear.h"

void MenuClear::init(RenderWindow& window)
{
    if (!_font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
    {
        cout << "No font is here" << endl;
        return;
    }

    // Clear
    _clearText.setFont(_font);
    _clearText.setFillColor(Color::White);
    _clearText.setString("Clear!");
    _clearText.setCharacterSize(60);
    _clearText.setOrigin(_clearText.getLocalBounds().width / 2, _clearText.getLocalBounds().height / 2);
    _clearText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 - 60);

    // Score
    _scoreText.setFont(_font);
    _scoreText.setFillColor(Color::White);
    _scoreText.setString("Score");
    _scoreText.setCharacterSize(60);
    _scoreText.setOrigin(_scoreText.getLocalBounds().width / 2, _scoreText.getLocalBounds().height / 2);
    _scoreText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 60);

    // Ok
	_okText.setFont(_font);
	_okText.setFillColor(Color::White);
	_okText.setString("OK");
	_okText.setCharacterSize(60);
	_okText.setOrigin(_okText.getLocalBounds().width / 2, _okText.getLocalBounds().height / 2);
	_okText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 180);
}

void MenuClear::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
       if (_okText.getGlobalBounds().contains(mousePos)) // OK 영역을 클릭한 경우
            nextState = MenuIndex::STAGE_MENU; // 스테이지 선택 메뉴로 돌아감
    }
}

void MenuClear::render(Renderer& renderer)
{
    
    renderer._target.draw(_clearText);

    // score를 현재 클리어한 스테이지 점수를 get한 후 출력
    int id = Game::getInstance().getPlayerId(); // 현재 player id
    int stage = Game::getInstance().getPlayerCurrentClearStage(id); // 현재 player가 클리어한 스테이지
    _scoreText.setString("Score : " + to_string(Game::getInstance().getPlayerStageScore(id, stage)));
    _scoreText.setOrigin(_scoreText.getLocalBounds().width / 2, _scoreText.getLocalBounds().height / 2); // text origin 재설정
    _scoreText.setPosition((float)renderer._target.getSize().x / 2, (float)renderer._target.getSize().y / 2 + 60); // text position 재설정
    renderer._target.draw(_scoreText);

    renderer._target.draw(_okText);
}
