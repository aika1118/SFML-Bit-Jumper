#pragma once

#include "Menu.h"
#include "Config.h"
#include "Game.h"
#include <vector>
#include <iostream>

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
};