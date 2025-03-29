#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include <mutex>

using namespace std;

struct RankData {
	string name;
	string score;
};

class MenuRanking : public Menu {
public:
	void init(RenderWindow& window) override;
	void update(RenderWindow& window, const Event& event, float deltaTime, int& nextState) override;
	void render(Renderer& renderer) override;
	void pushRankData(string name, string score);
	void initRankData();

private:
	Font _font;
	int _currentPage = 0;
	Text _rankingText;
	Text _prevPageText;
	Text _nextPageText;
	Text _returnPageText;
	Text _loadingText;

	vector<RankData> _rankData;

	mutex rankData_mutex_; // rankData가 네트워크 쓰레드 콜백 함수에서 수정이 이루어지고 메인 쓰레드에서 읽기 작업이 있기 때문에 lock으로 보호 필요
};