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
	static bool getPacketWaitStatus(PacketType packetType);
	static void setPacketWaitStatus(PacketType packetType, bool status);

	// inline static���� ������Ͽ��� ����� ���ÿ� �ʱ�ȭ���� ����
	inline static unordered_map<PacketType, bool> _PacketWaitStatus; // �� PACKET ��û�� ���� ��ٸ��� �ִ��� ���¸� ���� (1 = ��ٸ�����, 0 = ��ٸ��� �ʴ���)
};