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

	// game.cpp에서 최신 clear stage 값을 아직 불러오지 못한 경우 바로 return 처리
	if (currentClearStage == SETTING_PLAYER_CURRENT_CLEAR_STAGE_NOT_INITED)
	{
		return;
	}
	
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		int canSelectMaxStage = min(currentClearStage + 1, (int)stageTexts.size() - 1); // 현재 클리어한 스테이지 + 1 이하의 스테이지만 선택 가능

		for (int i = 0; i <= canSelectMaxStage; ++i) // 도전 가능한 스테이지 까지만 선택 가능
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

	// game.cpp에서 최신 clear stage 값을 아직 불러오지 못한 경우 로딩중 화면 처리
	if (currentClearStage == SETTING_PLAYER_CURRENT_CLEAR_STAGE_NOT_INITED) 
	{
		// Loading... text 출력 필요
		return;
	}

	for (int i = 0; i < stageTexts.size(); ++i)
	{
		if (i > currentClearStage + 1) stageTexts[i].setFillColor(Color(100, 100, 100)); // 현재 클리어한 스테이지 + 2 부터는 선택 불가하므로 Text 음영처리
		else stageTexts[i].setFillColor(Color::White);

		renderer._target.draw(stageTexts[i]);
	}
}
