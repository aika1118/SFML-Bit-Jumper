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
	ifstream inputFile(SETTING_CONFIG_FILE); // scope ����� �ڵ� closing
	
	// SETTING_CONFIG_FILE�� ���� ��� ���� ����
	if (!inputFile)
	{
		// ���� ���ϸ� �����ϰ� uid�� -1�� return
		// ���� �г��� �Է� �޴����� �г����� �Է��� ��쿡 uid �߱� ����
		json config;
		config["UID"] = SETTING_UID_NOT_INITED;

		ofstream outputFile(SETTING_CONFIG_FILE); // scope ����� �ڵ� closing
		outputFile << config.dump(4); // pretty print�� ����
		cout << "config.json ������ �����Ǿ����ϴ�. " << endl;

		return SETTING_UID_NOT_INITED;
	}

	// SETTING_CONFIG_FILE�� �̹� �����ϴ� ���
	try
	{
		json config;
		inputFile >> config;

		if (config.contains("UID")) return config["UID"];

		return SETTING_UID_NOT_INITED; // json ���Ͽ� UID key�� ���� ���
	}
	catch (const exception& e)
	{
		cerr << "json file read error !" << endl;
		cerr << "error message: " << e.what() << endl;
		cout << "init json file..." << endl;

		json config;
		config["UID"] = SETTING_UID_NOT_INITED;
		ofstream outputFile(SETTING_CONFIG_FILE); // scope ����� �ڵ� closing
		outputFile << config.dump(4); // pretty print�� ����

		return SETTING_UID_NOT_INITED;
	}
}

void Util::setUID(int uid)
{
	ifstream inputFile(SETTING_CONFIG_FILE); // scope ����� �ڵ� closing

	// SETTING_CONFIG_FILE�� ���� ��� ���� ����
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
	ofstream outputFile(SETTING_CONFIG_FILE); // scope ����� �ڵ� closing
	outputFile << config.dump(4); // pretty print�� ����

	// MAKE_USERNAME_MENU�� ������, Game Ŭ������ update���� Main �޴��� �Ѿ�� ���� �۾�
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




