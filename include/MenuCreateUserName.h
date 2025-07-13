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
	/**
	 * ���� �޽����� �����մϴ�.
	 *
	 * �� �Լ��� ���� �޽����� �����ϱ� ���� ���˴ϴ�.
	 *
	 * @param text ���� �޽����Դϴ�.
	 */
	void setErrorText(string text);

private:
	/**
	 * ����� ID ���� ���θ� ��ȯ�մϴ�.
	 *
	 * �� �Լ��� ����� ID ���� ���θ� ��ȯ�ϱ� ���� ���˴ϴ�.
	 *
	 * @return ����� ID ���� �����Դϴ�.
	 */
	bool getIsUidCreated() const;
	/**
	 * ����� ID ���� ���θ� �����մϴ�.
	 *
	 * �� �Լ��� ����� ID ���� ���θ� �����ϱ� ���� ���˴ϴ�.
	 *
	 * @param status ����� ID ���� �����Դϴ�.
	 */
	void setIsUidCreated(bool status);
	/**
	 * �Է� ���ڿ��� ��� �ִ��� ���θ� ��ȯ�մϴ�.
	 *
	 * �� �Լ��� �Է� ���ڿ��� ��� �ִ��� ���θ� ��ȯ�ϱ� ���� ���˴ϴ�.
	 *
	 * @return �Է� ���ڿ��� ��� �ִ��� �����Դϴ�.
	 */
	bool isInputStringEmpty();


	Font _font;
	Text _infoText;
	RectangleShape _inputBox;
	string _inputString; // ���� �����忡�� ����, ��Ʈ��ũ �����忡�� �б� �� ��Ŷ ��û�� ���� �ֱ� ������ lock���� ��ȣ �ʿ�
	Text _cursorText;
	Text _inputText;
	Text _submitText;
	RectangleShape _submitBox;
	Text _errorText;
	Text _loadingText;
	bool _cursorOn = false;

	bool _isUidCreateRequestSended = false;
	bool _isUidCreated = false; // ���� �����忡�� �б�, ��Ʈ��ũ �����忡�� ���� �۾��� �����ؼ� lock���� ��ȣ �ʿ�
	
	mutable mutex isUidCreated_mutex_; // const �Լ����� lock ���� �������� mutable ����
	mutex inputString_mutex_;


};