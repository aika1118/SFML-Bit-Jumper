#include "MenuRanking.h"

void MenuRanking::init(RenderWindow& window)
{
    if (!_font.loadFromFile("./resources/Fonts/ttf/BMDOHYEON_ttf.ttf"))
    {
        cout << "No font is here" << endl;
        return;
    }

    // "Ranking" 텍스트
    _rankingText.setFont(_font);
    _rankingText.setFillColor(Color::White);
    _rankingText.setString("Ranking");
    _rankingText.setCharacterSize(40);
    _rankingText.setOrigin(_rankingText.getLocalBounds().width / 2, _rankingText.getLocalBounds().height / 2);
    _rankingText.setPosition((float)window.getSize().x / 2, 50.f);

    // 이전 "<<" 텍스트
    _prevPageText.setFont(_font);
    _prevPageText.setFillColor(Color::White);
    _prevPageText.setString("<<");
    _prevPageText.setCharacterSize(40);
    _prevPageText.setOrigin(_prevPageText.getLocalBounds().width / 2, _prevPageText.getLocalBounds().height / 2);
    _prevPageText.setPosition((float)window.getSize().x / 2 - 200.f, (float)window.getSize().y - 150.f);

    // 다음 ">>" 텍스트
    _nextPageText.setFont(_font);
    _nextPageText.setFillColor(Color::White);
    _nextPageText.setString(">>");
    _nextPageText.setCharacterSize(40);
    _nextPageText.setOrigin(_nextPageText.getLocalBounds().width / 2, _nextPageText.getLocalBounds().height / 2);
    _nextPageText.setPosition((float)window.getSize().x / 2 + 200.f, (float)window.getSize().y - 150.f);

    // 돌아가기 텍스트
    _returnPageText.setFont(_font);
    _returnPageText.setFillColor(Color::White);
    _returnPageText.setString("Return");
    _returnPageText.setCharacterSize(30);
    _returnPageText.setOrigin(_returnPageText.getLocalBounds().width / 2, _returnPageText.getLocalBounds().height / 2);
    _returnPageText.setPosition(100.f, (float)window.getSize().y - 50.f);

    // Loading... 텍스트
    _loadingText.setFont(_font);
    _loadingText.setFillColor(Color::White);
    _loadingText.setString("Loading...");
    _loadingText.setCharacterSize(40);
    _loadingText.setOrigin(_loadingText.getLocalBounds().width / 2, _loadingText.getLocalBounds().height / 2);
    _loadingText.setPosition((float)window.getSize().x / 2, (float)window.getSize().y / 2);

}

void MenuRanking::update(RenderWindow& window, const Event& event, float deltaTime, int& nextState)
{
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        // 이전 페이지로 이동 
        if (_prevPageText.getGlobalBounds().contains(mousePos) && _currentPage > 0) 
        {
            _currentPage--;
            return;
        }

        {
            lock_guard<mutex> lock(rankData_mutex_);
            // 다음 페이지로 이동 (남은 랭킹 데이터가 있는 경우)
            if (_nextPageText.getGlobalBounds().contains(mousePos) && (_currentPage + 1) * MENU_RANKING_PER_PAGE < _rankData.size()) 
            {
                _currentPage++; 
                return;
            }
        }

        // 이전 메뉴로 이동
        if (_returnPageText.getGlobalBounds().contains(mousePos)) 
		{
			nextState = MenuIndex::MAIN_MENU;
            initRankData(); // 랭킹 데이터 초기화
			return;
		}
    }
}

void MenuRanking::render(Renderer& renderer)
{
    renderer._target.draw(_rankingText);
    renderer._target.draw(_prevPageText);
    renderer._target.draw(_nextPageText);
    renderer._target.draw(_returnPageText);

    lock_guard<mutex> lock(rankData_mutex_);

    // 랭킹 데이터가 아직 비어있는 경우 (서버에서 아직 데이터를 불러오는 중이라면)
    if (_rankData.size() == 0)
    {
        renderer._target.draw(_loadingText);
        return;
    }

    int startIdx = _currentPage * MENU_RANKING_PER_PAGE;
	int endIdx = min(startIdx + MENU_RANKING_PER_PAGE, (int)_rankData.size()); // endIdx는 rank 데이터 개수를 넘지 않도록 설정

    for (int i = startIdx; i < endIdx; ++i)
    {
        Text text;
        text.setFont(_font);
        text.setString(to_string(i+1) + ". " + _rankData[i].name + " - " + _rankData[i].score);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setCharacterSize(30);
        text.setFillColor(Color::White);
        text.setPosition((float)renderer._target.getSize().x / 2, (float)(200 + (i - startIdx) * 80));
        renderer._target.draw(text);
    }

}

void MenuRanking::pushRankData(string name, string score)
{
    lock_guard<mutex> lock(rankData_mutex_);
    _rankData.push_back({name, score});
    return;
}

void MenuRanking::initRankData()
{
    cout << "initRankData() Called !" << endl;
    lock_guard<mutex> lock(rankData_mutex_);
    _rankData.clear();
    return;
}


