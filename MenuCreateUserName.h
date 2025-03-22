#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>

using namespace std;

class MenuCreateUserName : public Menu {
public:
	void init(RenderWindow& window) override;
	void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	void render(Renderer& renderer) override;
	void setErrorText(string text);

private:
	Font _font;
	Text _infoText;
	RectangleShape _inputBox;
	string _inputString;
	Text _cursorText;
	Text _inputText;
	Text _submitText;
	RectangleShape _submitBox;
	Text _errorText;
	Text _loadingText;
	bool _cursorOn = false;

	bool _isUidCreateRequestSended = false;
	bool _isUidCreated = false;
};