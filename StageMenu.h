#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>

using namespace std;

class StageMenu : public Menu {
public:
    void init(RenderWindow& window) override;
    void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
    void render(Renderer& renderer) override;

private:
    Font _font;
    vector<Text> stageTexts;
    Text _returnPageText;
    Text _loadingText;
};