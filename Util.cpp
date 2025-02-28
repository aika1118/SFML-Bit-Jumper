#include "Util.h"
#include "Game.h"

string Util::floatToString(float num, int digit)
{
	stringstream ss;
	ss << fixed << setprecision(digit) << num;
	return ss.str();
}

int Util::getUID()
{
	ifstream inputFile(SETTING_CONFIG_FILE); // scope 벗어나면 자동 closing
	
	// SETTING_CONFIG_FILE이 없는 경우 새로 생성
	if (!inputFile)
	{
		// 설정 파일만 생성하고 uid는 -1로 return
		// 이후 닉네임 입력 메뉴에서 닉네임을 입력한 경우에 uid 발급 진행
		json config;
		config["UID"] = SETTING_UID_NOT_INITED;

		ofstream outputFile(SETTING_CONFIG_FILE); // scope 벗어나면 자동 closing
		outputFile << config.dump(4); // pretty print로 저장
		cout << "config.json 파일이 생성되었습니다. " << endl;

		return SETTING_UID_NOT_INITED;
	}

	// SETTING_CONFIG_FILE이 이미 존재하는 경우
	try
	{
		json config;
		inputFile >> config;

		if (config.contains("UID")) return config["UID"];

		return SETTING_UID_NOT_INITED; // json 파일에 UID key가 없는 경우
	}
	catch (const exception& e)
	{
		cerr << "json file read error !" << endl;
		cerr << "error message: " << e.what() << endl;
		cout << "init json file..." << endl;

		json config;
		config["UID"] = SETTING_UID_NOT_INITED;
		ofstream outputFile(SETTING_CONFIG_FILE); // scope 벗어나면 자동 closing
		outputFile << config.dump(4); // pretty print로 저장

		return SETTING_UID_NOT_INITED;
	}
}

void Util::setUID(int uid)
{
	ifstream inputFile(SETTING_CONFIG_FILE); // scope 벗어나면 자동 closing

	// SETTING_CONFIG_FILE이 없는 경우 새로 생성
	if (!inputFile)
	{
		cerr << "There is no config" << endl;
		return;
	}

	json config;
	inputFile >> config;
	if (!config.contains("UID"))
	{
		cerr << "There is no UID key in config file" << endl;
		return;
	}

	config["UID"] = uid;
	ofstream outputFile(SETTING_CONFIG_FILE); // scope 벗어나면 자동 closing
	outputFile << config.dump(4); // pretty print로 저장

	// MAKE_USERNAME_MENU에 있지만, Game 클래스의 update에서 Main 메뉴로 넘어가기 위한 작업
	Game::getInstance().setMenuState(MenuIndex::NONE); 
}



string Util::getUserName(int uid)
{
	return string();
}

bool Util::isServerConnected()
{
	if (Game::getInstance().getClient() == nullptr) return false;
	if (SERVER_USING_CONNECTION == false) return false;

	return true;
}




