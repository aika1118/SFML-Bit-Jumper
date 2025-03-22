#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "json.hpp"
#include "Config.h"
#include "Packet.h"

using namespace std;
using namespace nlohmann;

class Util
{
public:
	// �Ҽ��� digit �ڸ�����ŭ string���� ��ȯ �� return 
	static string floatToString(float num, int digit);

	static int getUID();
	static void setUID(int uid);
	static string getUserName(int uid);

	static bool checkServerConnection();
	static bool getPacketWaitStatus(int packet_id);
	static void setPacketWaitStatus(int packet_id, bool status);

	inline static vector<bool> _PacketWaitStatus = vector<bool>(PAKCET_TYPE_MAX_NUM, false); // �� PACKET ��û�� ���� ��ٸ��� �ִ��� ���¸� �����ϴ� vector (1 = ��ٸ�����, 0 = ��ٸ��� �ʴ���)
};