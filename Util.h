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
	static bool getPacketWaitStatus(PacketType packetType);
	static void setPacketWaitStatus(PacketType packetType, bool status);

	// inline static으로 헤더파일에서 선언과 동시에 초기화까지 진행
	inline static unordered_map<PacketType, bool> _PacketWaitStatus; // 각 PACKET 요청에 대해 기다리고 있는지 상태를 저장 (1 = 기다리는중, 0 = 기다리지 않는중)
};