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
	 * ��ŷ �����͸� �߰��մϴ�.
	 *
	 * �� �Լ��� ��ŷ �����͸� �߰��ϱ� ���� ���˴ϴ�.
	 *
	 * @param name ����� �̸��Դϴ�.
	 * @param score ����� �����Դϴ�.
	 */
	void pushRankData(string name, string score);
	/**
	 * ��ŷ �����͸� �ʱ�ȭ�մϴ�.
	 *
	 * �� �Լ��� ��ŷ �����͸� �ʱ�ȭ�ϱ� ���� ���˴ϴ�.
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

	vector<RankData> _rankData; // ��ŷ ���� ����

	mutex rankData_mutex_; // rankData�� ��Ʈ��ũ ������ �ݹ� �Լ����� ������ �̷������ ���� �����忡�� �б� �۾��� �ֱ� ������ lock���� ��ȣ �ʿ�
};