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
	ifstream inputFile(SETTING_CONFIG_FILE); // �ش� ��ü�� scope ����� �ڵ� closing
	
	// SETTING_CONFIG_FILE�� ���� ��� ���� ����
	if (!inputFile)
	{
		// ���� ���ϸ� �����ϰ� uid�� -1�� return
		// ���� �г��� �Է� �޴����� �г����� �Է��� ��쿡 uid �߱� ����
		json config;
		config["UID"] = SETTING_UID_NOT_INITED;

		ofstream outputFile(SETTING_CONFIG_FILE); // �ش� ��ü�� scope ����� �ڵ� closing
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
	// �ʿ� �� ���� ����
	return string();
}

bool Util::checkServerConnection()
{
	if (SERVER_OFFLINE_MODE == true) return false;


	try
	{
		io_context io_context;

		// TCP ��������Ʈ ����
		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints = resolver.resolve(SERVER_IP, SERVER_PORT);

		// ���� ����
		tcp::socket socket(io_context);

		// ���� �õ� (Ÿ�Ӿƿ� ���� �⺻ ����)
		connect(socket, endpoints);

		// ���� ���� �� ���� �ݱ�
		socket.close();
		return true;
	}
	catch (const exception& e)
	{
		cerr << "Exception: " << e.what() << "\n"; // ����ó��
		return false;
	}
	
}

bool Util::getPacketWaitStatus(PacketType packetType)
{
	return _PacketWaitStatus[packetType];
}

void Util::setPacketWaitStatus(PacketType packetType, bool status)
{
	_PacketWaitStatus[packetType] = status;
}

void Util::stopSounds()
{
	for (auto& music : Resources::_musics) music.second.stop();
	for (auto& sound : Resources::_sounds) sound.second.stop();
}




