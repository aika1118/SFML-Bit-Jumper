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
	// 소숫점 digit 자리수만큼 string으로 변환 후 return 
	static string floatToString(float num, int digit);

	static int getUID();
	static void setUID(int uid);
	static string getUserName(int uid);

	static bool checkServerConnection();
	static bool getPacketWaitStatus(int packet_id);
	static void setPacketWaitStatus(int packet_id, bool status);

	inline static vector<bool> _PacketWaitStatus = vector<bool>(PAKCET_TYPE_MAX_NUM, false); // 각 PACKET 요청에 대해 기다리고 있는지 상태를 저장하는 vector (1 = 기다리는중, 0 = 기다리지 않는중)
};