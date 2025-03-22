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
    _clearText.setCharacterSize(50);
    _clearText.setOrigin(_clearText.getLocalBounds().width / 2, _clearText.getLocalBounds().height / 2);
    _clearText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 - 100);

    // Score
    _scoreText.setFont(_font);
    _scoreText.setFillColor(Color::White);
    _scoreText.setString("Score");
    _scoreText.setCharacterSize(50);
    _scoreText.setOrigin(_scoreText.getLocalBounds().width / 2, _scoreText.getLocalBounds().height / 2);
    _scoreText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 20);

    // Ok
	_okText.setFont(_font);
	_okText.setFillColor(Color::White);
	_okText.setString("OK");
	_okText.setCharacterSize(50);
	_okText.setOrigin(_okText.getLocalBounds().width / 2, _okText.getLocalBounds().height / 2);
	_okText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 140);

    // Loading... �ؽ�Ʈ
    _loadingText.setFont(_font);
    _loadingText.setFillColor(Color::White);
    _loadingText.setString("Loading...");
    _loadingText.setCharacterSize(40);
    _loadingText.setOrigin(_loadingText.getLocalBounds().width / 2, _loadingText.getLocalBounds().height / 2);
    _loadingText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2);
}

void MenuClear::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    // ���� ��Ŷ ��û�� ���� wait ������ ��� �ε��� ó��
    if (Util::getPacketWaitStatus(PACKET_WRITE) == true) return;

    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
       if (_okText.getGlobalBounds().contains(mousePos)) // OK ������ Ŭ���� ���
            nextState = MenuIndex::STAGE_MENU; // �������� ���� �޴��� ���ư�
    }
}

void MenuClear::render(Renderer& renderer)
{
    if (Util::getPacketWaitStatus(PACKET_WRITE) == true) // ���� ��Ŷ ��û�� ���� wait ������ ��� �ε��� ó��
    {
        renderer._target.draw(_loadingText);
        return;
    }

    renderer._target.draw(_clearText);

    // score�� ���� Ŭ������ �������� ������ get�� �� ���
    int id = Game::getInstance().getUid(); // ���� player id
    int stage = Game::getInstance().getPlayerCurrentClearStage(id); // ���� player�� Ŭ������ ��������
    _scoreText.setString("Score : " + to_string(Game::getInstance().getPlayerStageScore(id, stage)));
    _scoreText.setOrigin(_scoreText.getLocalBounds().width / 2, _scoreText.getLocalBounds().height / 2); // text origin �缳��
    _scoreText.setPosition((float)renderer._target.getSize().x / 2, (float)renderer._target.getSize().y / 2 + 20); // text position �缳��
    renderer._target.draw(_scoreText);

    renderer._target.draw(_okText);
}
