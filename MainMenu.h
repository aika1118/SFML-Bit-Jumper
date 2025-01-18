#pragma once

#include "Menu.h"
#include "Config.h"
#include <iostream>

using namespace std;

class MainMenu : public Menu
{
public:
	void init(RenderWindow& window) override;
	void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	void render(Renderer& renderer) override;

private:
	Font _font;
	Text _playText;
	Text _exitText;
};