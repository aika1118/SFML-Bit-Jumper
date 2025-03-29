#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include <mutex>

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
	string _inputString; // 메인 쓰레드에서 수정, 네트워크 쓰레드에서 읽기 후 패킷 요청이 들어가고 있기 때문에 lock으로 보호 필요
	Text _cursorText;
	Text _inputText;
	Text _submitText;
	RectangleShape _submitBox;
	Text _errorText;
	Text _loadingText;
	bool _cursorOn = false;

	bool _isUidCreateRequestSended = false;
	bool _isUidCreated = false; // 메인 쓰레드에서 읽기, 네트워크 쓰레드에서 쓰기 작업을 진행해서 lock으로 보호 필요

	bool isInputStringEmpty();
	
	mutable mutex isUidCreated_mutex_; // const 함수에서 lock 상태 변경위해 mutable 선언
	mutex inputString_mutex_;

	bool getIsUidCreated() const;
	void setIsUidCreated(bool status);
};