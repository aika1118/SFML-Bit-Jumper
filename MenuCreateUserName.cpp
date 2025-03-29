#include "MenuCreateUserName.h"

void MenuCreateUserName::init(RenderWindow& window)
{
    if (!_font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
    {
        cout << "No font is here" << endl;
        return;
    }

    // "닉네임을 입력하세요." 텍스트
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

    // inputText (inputbox에 표시될 Text)
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

    // Loading... 텍스트
    _loadingText.setFont(_font);
    _loadingText.setFillColor(Color::White);
    _loadingText.setString("Loading...");
    _loadingText.setCharacterSize(40);
    _loadingText.setOrigin(_loadingText.getLocalBounds().width / 2, _loadingText.getLocalBounds().height / 2);
    _loadingText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2);
}

void MenuCreateUserName::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    // uid 생성 요청 보냈고 아직 서버에서 응답을 받지 못하였다면 로딩중 처리
    if (_isUidCreateRequestSended && !getIsUidCreated()) return;
        
    // inputBox 영역을 클릭한 경우 cursor 표시를 위한 cursorOn = true 처리
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (_inputBox.getGlobalBounds().contains(mousePos))
        {
            _cursorOn = true;
            return;
        }
        
        _cursorOn = false;

        // submit 버튼을 누른 경우 submit 처리
        if (_submitBox.getGlobalBounds().contains(mousePos))
        {
            if (isInputStringEmpty())
			{
				cout << "cannot submit empty string" << endl; // 차후 인게임에서 확인할 수 있도록 변경 필요
				return;
			}

            cout << "submit success: " << _inputString << endl;
            // Server에 닉네임 저장 및 uid 발급을 위한 패킷전송
            if (Game::getInstance().isServerConnected())
                _isUidCreateRequestSended = true;

                lock_guard<mutex> lock(inputString_mutex_); // 패킷 요청 보내는 쓰레드가 공유자원(inputString) 읽을 때 lock 적용
                Game::getInstance().getClient()->send_packet_async(PACKET_CREATE, _inputString,
                    [this](const string& response) { // 콜백 정의
                        // uid 저장을 위해 Util::setUID() 호출
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

    // 키보드 타이핑이 감지된 경우 inputText에 키보드 입력을 담기
    if (event.type == Event::TextEntered)
    {
        if (!_cursorOn) return; // 커서 포커스 되어있을 때만 입력처리

        // 백스페이스 처리
        if (event.text.unicode == 8) 
        {
            lock_guard<mutex> lock(inputString_mutex_);
            if (_inputString.empty()) return;

            _inputString.pop_back();
            _inputText.setString(_inputString);
            return;
        }

        // 스페이스 처리 (스페이스는 이름에 허용하지 않음)
        if (event.text.unicode == 32) return;

        // 일반 문자 입력 (6글자 제한)
        lock_guard<mutex> lock(inputString_mutex_);
        if (event.text.unicode < 128 && _inputString.size() < SETTING_USERNAME_MAX_LENGTH)
		{
            // _errorText 초기화
            _errorText.setString("");
           
			_inputString += (char)event.text.unicode;
			_inputText.setString(_inputString);
            return;
		}
    }
}

void MenuCreateUserName::render(Renderer& renderer)
{
    if (_isUidCreateRequestSended && !getIsUidCreated()) // uid 생성 요청 보냈고 아직 서버에서 응답을 받지 못하였다면 로딩중처리
    {
        renderer._target.draw(_loadingText);
        return;
    }
    renderer._target.draw(_infoText);
    renderer._target.draw(_inputBox);
    renderer._target.draw(_inputText);
    renderer._target.draw(_submitText);
    renderer._target.draw(_submitBox);

    // 에러메세지는 서버 요청 후 string이 써지기 때문에 origin 및 position 재조정
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
