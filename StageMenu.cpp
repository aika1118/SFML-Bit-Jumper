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
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		for (int i = 0; i < stageTexts.size(); ++i)
		{
			if (stageTexts[i].getGlobalBounds().contains(mousePos))
			{
				nextState = StageIndex::START_OF_STAGE_INDEX + (i + 1); // Stage i+i ½ÃÀÛ
				cout << "Stage " << i + 1 << " Selected" << endl;
				break;
			}
		}
	}
}

void StageMenu::render(Renderer& renderer)
{
	for (const auto& stageText : stageTexts)
	{
		renderer._target.draw(stageText);
	}
}
