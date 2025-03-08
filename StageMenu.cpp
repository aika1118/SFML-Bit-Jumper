#include "StageMenu.h"

void StageMenu::init(RenderWindow& window)
{
	if (!_font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
	{
		cout << "No font is here" << endl;
		return;
	}

	for (int i = 0; i < MENU_MAX_STAGE_NUM; ++i)
	{
		Text stageText;
		stageText.setFont(_font);
		stageText.setFillColor(Color::White);
		stageText.setString("Stage " + to_string(i + 1));
		stageText.setCharacterSize(40);
		stageText.setOrigin(stageText.getLocalBounds().width / 2, stageText.getLocalBounds().height / 2);
		stageText.setPosition((float)window.getSize().x / 2.f, (float)window.getSize().y / (MENU_MAX_STAGE_NUM + 1) * (i + 1));
		stageTexts.push_back(stageText);
	}
}

void StageMenu::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
	int currentClearStage = Game::getInstance().getPlayerCurrentClearStage(Game::getInstance().getUid());

	// game.cpp���� �ֽ� clear stage ���� ���� �ҷ����� ���� ��� �ٷ� return ó��
	if (currentClearStage == SETTING_PLAYER_CURRENT_CLEAR_STAGE_NOT_INITED)
	{
		return;
	}
	
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		int canSelectMaxStage = min(currentClearStage + 1, (int)stageTexts.size() - 1); // ���� Ŭ������ �������� + 1 ������ ���������� ���� ����

		for (int i = 0; i <= canSelectMaxStage; ++i) // ���� ������ �������� ������ ���� ����
		{
			if (stageTexts[i].getGlobalBounds().contains(mousePos))
			{
				nextState = MenuIndex::IN_PLAY; 
				Game::getInstance().setStageSelected(i);
				Game::getInstance().setMapImage(i);

				cout << "Stage " << i + 1 << " Selected" << endl;
				break;
			}
		}
	}
}

void StageMenu::render(Renderer& renderer)
{
	int currentClearStage = Game::getInstance().getPlayerCurrentClearStage(Game::getInstance().getUid());

	// game.cpp���� �ֽ� clear stage ���� ���� �ҷ����� ���� ��� �ε��� ȭ�� ó��
	if (currentClearStage == SETTING_PLAYER_CURRENT_CLEAR_STAGE_NOT_INITED) 
	{
		// Loading... text ��� �ʿ�
		return;
	}

	for (int i = 0; i < stageTexts.size(); ++i)
	{
		if (i > currentClearStage + 1) stageTexts[i].setFillColor(Color(100, 100, 100)); // ���� Ŭ������ �������� + 2 ���ʹ� ���� �Ұ��ϹǷ� Text ����ó��
		else stageTexts[i].setFillColor(Color::White);

		renderer._target.draw(stageTexts[i]);
	}
}
