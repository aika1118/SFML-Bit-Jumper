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
	/**
	 * 랭킹 데이터를 추가합니다.
	 *
	 * 이 함수는 랭킹 데이터를 추가하기 위해 사용됩니다.
	 *
	 * @param name 사용자 이름입니다.
	 * @param score 사용자 점수입니다.
	 */
	void pushRankData(string name, string score);
	/**
	 * 랭킹 데이터를 초기화합니다.
	 *
	 * 이 함수는 랭킹 데이터를 초기화하기 위해 사용됩니다.
	 */
	void initRankData();

private:
	Font _font;
	int _currentPage = 0;
	Text _rankingText;
	Text _prevPageText;
	Text _nextPageText;
	Text _returnPageText;
	Text _loadingText;

	vector<RankData> _rankData; // 랭킹 정보 저장

	mutex rankData_mutex_; // rankData가 네트워크 쓰레드 콜백 함수에서 수정이 이루어지고 메인 쓰레드에서 읽기 작업이 있기 때문에 lock으로 보호 필요
};