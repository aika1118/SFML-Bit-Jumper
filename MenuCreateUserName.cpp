#include "MenuCreateUserName.h"

void MenuCreateUserName::init(RenderWindow& window)
{
    if (!_font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
    {
        cout << "No font is here" << endl;
        return;
    }

    // "�г����� �Է��ϼ���." �ؽ�Ʈ
    _infoText.setFont(_font);
    _infoText.setFillColor(Color::White);
    _infoText.setString("Create a username");
    _infoText.setCharacterSize(40);
    _infoText.setOrigin(_infoText.getLocalBounds().width / 2, _infoText.getLocalBounds().height / 2);
    _infoText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 - 70.f);

    // inputbox
	_inputBox.setSize(Vector2f(300.f, 50.f));
	_inputBox.setFillColor(Color::White);
	_inputBox.setOrigin(_inputBox.getLocalBounds().width / 2, _inputBox.getLocalBounds().height / 2);
	_inputBox.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2);

    // inputText (inputbox�� ǥ�õ� Text)
	_inputText.setFont(_font);
	_inputText.setFillColor(Color::Black);
	_inputText.setCharacterSize(40);
	_inputText.setOrigin(_inputText.getLocalBounds().width / 2, _inputText.getLocalBounds().height / 2);
	_inputText.setPosition((float)window.getSize().x / 2 - (float)_inputBox.getLocalBounds().width / 2 + 15.f, (float)window.getSize().y / 2 - 28.f);

    // cursorText
	_cursorText.setFont(_font);
	_cursorText.setFillColor(Color::Black);
	_cursorText.setString("|");
	_cursorText.setCharacterSize(40);
	_cursorText.setOrigin(_cursorText.getLocalBounds().width / 2, _cursorText.getLocalBounds().height / 2);
	_cursorText.setPosition((float)window.getSize().x / 2 - (float)_inputBox.getLocalBounds().width / 2 + 5.f, (float)window.getSize().y / 2 - 10.f);

    // submitText
    _submitText.setFont(_font);
    _submitText.setFillColor(Color::White);
    _submitText.setString("Submit");
    _submitText.setCharacterSize(40);
    _submitText.setOrigin(_submitText.getLocalBounds().width / 2, _submitText.getLocalBounds().height / 2);
    _submitText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 100.f);

    // submitBox
	_submitBox.setSize(Vector2f(180.f, 50.f));
    _submitBox.setFillColor(Color::Transparent);
	_submitBox.setOutlineColor(Color::White);
    _submitBox.setOutlineThickness(2.f);
	_submitBox.setOrigin(_submitBox.getLocalBounds().width / 2, _submitBox.getLocalBounds().height / 2);
	_submitBox.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 115.f);

    // errorText
	_errorText.setFont(_font);
	_errorText.setFillColor(Color::Red);
	_errorText.setCharacterSize(20);
	_errorText.setOrigin(_errorText.getLocalBounds().width / 2, _errorText.getLocalBounds().height / 2);
	_errorText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2 + 200.f);

    // Loading... �ؽ�Ʈ
    _loadingText.setFont(_font);
    _loadingText.setFillColor(Color::White);
    _loadingText.setString("Loading...");
    _loadingText.setCharacterSize(40);
    _loadingText.setOrigin(_loadingText.getLocalBounds().width / 2, _loadingText.getLocalBounds().height / 2);
    _loadingText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2);
}

void MenuCreateUserName::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    // uid ���� ��û ���°� ���� �������� ������ ���� ���Ͽ��ٸ� �ε��� ó��
    if (_isUidCreateRequestSended && !getIsUidCreated()) return;
        
    // inputBox ������ Ŭ���� ��� cursor ǥ�ø� ���� cursorOn = true ó��
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (_inputBox.getGlobalBounds().contains(mousePos))
        {
            _cursorOn = true;
            return;
        }
        
        _cursorOn = false;

        // submit ��ư�� ���� ��� submit ó��
        if (_submitBox.getGlobalBounds().contains(mousePos))
        {
            if (isInputStringEmpty())
			{
				cout << "cannot submit empty string" << endl; // ���� �ΰ��ӿ��� Ȯ���� �� �ֵ��� ���� �ʿ�
				return;
			}

            cout << "submit success: " << _inputString << endl;
            // Server�� �г��� ���� �� uid �߱��� ���� ��Ŷ����
            if (Game::getInstance().isServerConnected())
                _isUidCreateRequestSended = true;

                lock_guard<mutex> lock(inputString_mutex_); // ��Ŷ ��û ������ �����尡 �����ڿ�(inputString) ���� �� lock ����
                Game::getInstance().getClient()->send_packet_async(PACKET_CREATE, _inputString,
                    [this](const string& response) { // �ݹ� ����
                        // uid ������ ���� Util::setUID() ȣ��
                        cout << "PACKET_CREATE callback!" << endl;
                        Util::setUID(stoi(response));
                        cout << "uid saved: " << Util::getUID() << endl;

                        setIsUidCreated(true);

                        return;
                    }
                );


            return;
        }

        return;
    }

    // Ű���� Ÿ������ ������ ��� inputText�� Ű���� �Է��� ���
    if (event.type == Event::TextEntered)
    {
        if (!_cursorOn) return; // Ŀ�� ��Ŀ�� �Ǿ����� ���� �Է�ó��

        // �齺���̽� ó��
        if (event.text.unicode == 8) 
        {
            lock_guard<mutex> lock(inputString_mutex_);
            if (_inputString.empty()) return;

            _inputString.pop_back();
            _inputText.setString(_inputString);
            return;
        }

        // �����̽� ó�� (�����̽��� �̸��� ������� ����)
        if (event.text.unicode == 32) return;

        // �Ϲ� ���� �Է� (6���� ����)
        lock_guard<mutex> lock(inputString_mutex_);
        if (event.text.unicode < 128 && _inputString.size() < SETTING_USERNAME_MAX_LENGTH)
		{
            // _errorText �ʱ�ȭ
            _errorText.setString("");
           
			_inputString += (char)event.text.unicode;
			_inputText.setString(_inputString);
            return;
		}
    }
}

void MenuCreateUserName::render(Renderer& renderer)
{
    if (_isUidCreateRequestSended && !getIsUidCreated()) // uid ���� ��û ���°� ���� �������� ������ ���� ���Ͽ��ٸ� �ε���ó��
    {
        renderer._target.draw(_loadingText);
        return;
    }
    renderer._target.draw(_infoText);
    renderer._target.draw(_inputBox);
    renderer._target.draw(_inputText);
    renderer._target.draw(_submitText);
    renderer._target.draw(_submitBox);

    // �����޼����� ���� ��û �� string�� ������ ������ origin �� position ������
    _errorText.setOrigin(_errorText.getLocalBounds().width / 2, _errorText.getLocalBounds().height / 2);
    _errorText.setPosition((float)renderer._target.getSize().x / 2, (float)renderer._target.getSize().y / 2 + 200.f);
    renderer._target.draw(_errorText);

    if (_cursorOn) renderer._target.draw(_cursorText);
}

void MenuCreateUserName::setErrorText(string text)
{
    _errorText.setString(text);
}

bool MenuCreateUserName::isInputStringEmpty()
{
    lock_guard<mutex> lock(inputString_mutex_);
    return _inputString.empty();
}

bool MenuCreateUserName::getIsUidCreated() const
{
    lock_guard<mutex> lock(isUidCreated_mutex_);
    return _isUidCreated;
}

void MenuCreateUserName::setIsUidCreated(bool status)
{
    lock_guard<mutex> lock(isUidCreated_mutex_);
    _isUidCreated = status;
}
