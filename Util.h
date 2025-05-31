// File: Util.h
// Purpose: 게임 내 사용하는 유틸 함수들

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
	/**
	* 소숫점 자리수만큼 float 값을 문자열로 변환합니다.
	* @param num 변환할 float 값
	* @param digit 소숫점 자리수
	* @return 변환된 문자열
	*/
	static string floatToString(float num, int digit);
	/**
	* 고유 식별자(UID)를 반환합니다.
	* @return 고유 식별자(UID)
	*/
	static int getUID();
	/**
	* 고유 식별자(UID)를 설정합니다.
	* @param uid 설정할 고유 식별자(UID)
	*/
	static void setUID(int uid);
	/**
	* 사용자 이름을 반환합니다.
	* @param uid 사용자 고유 식별자(UID)
	* @return 사용자 이름
	*/
	static string getUserName(int uid);
	/**
	* 서버 연결 상태를 확인합니다.
	* @return 서버 연결 상태 (true: 연결됨, false: 연결되지 않음)
	*/
	static bool checkServerConnection();
	/**
	* 패킷 대기 상태를 반환합니다.
	* @param packetType 패킷 타입
	* @return 패킷 대기 상태 (true: 대기중, false: 대기하지 않음)
	*/
	static bool getPacketWaitStatus(PacketType packetType);
	/**
	* 패킷 대기 상태를 설정합니다.
	* @param packetType 패킷 타입
	* @param status 패킷 대기 상태 (true: 대기중, false: 대기하지 않음)
	*/
	static void setPacketWaitStatus(PacketType packetType, bool status);

	// static inline 으로 헤더파일에서 선언과 동시에 초기화까지 진행
	static inline unordered_map<PacketType, bool> _PacketWaitStatus; // 각 PACKET 요청에 대해 기다리고 있는지 상태를 저장 (1 = 기다리는중, 0 = 기다리지 않는중)
};