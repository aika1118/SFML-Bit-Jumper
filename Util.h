// File: Util.h
// Purpose: ���� �� ����ϴ� ��ƿ �Լ���

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
	* �Ҽ��� �ڸ�����ŭ float ���� ���ڿ��� ��ȯ�մϴ�.
	* @param num ��ȯ�� float ��
	* @param digit �Ҽ��� �ڸ���
	* @return ��ȯ�� ���ڿ�
	*/
	static string floatToString(float num, int digit);
	/**
	* ���� �ĺ���(UID)�� ��ȯ�մϴ�.
	* @return ���� �ĺ���(UID)
	*/
	static int getUID();
	/**
	* ���� �ĺ���(UID)�� �����մϴ�.
	* @param uid ������ ���� �ĺ���(UID)
	*/
	static void setUID(int uid);
	/**
	* ����� �̸��� ��ȯ�մϴ�.
	* @param uid ����� ���� �ĺ���(UID)
	* @return ����� �̸�
	*/
	static string getUserName(int uid);
	/**
	* ���� ���� ���¸� Ȯ���մϴ�.
	* @return ���� ���� ���� (true: �����, false: ������� ����)
	*/
	static bool checkServerConnection();
	/**
	* ��Ŷ ��� ���¸� ��ȯ�մϴ�.
	* @param packetType ��Ŷ Ÿ��
	* @return ��Ŷ ��� ���� (true: �����, false: ������� ����)
	*/
	static bool getPacketWaitStatus(PacketType packetType);
	/**
	* ��Ŷ ��� ���¸� �����մϴ�.
	* @param packetType ��Ŷ Ÿ��
	* @param status ��Ŷ ��� ���� (true: �����, false: ������� ����)
	*/
	static void setPacketWaitStatus(PacketType packetType, bool status);

	// static inline ���� ������Ͽ��� ����� ���ÿ� �ʱ�ȭ���� ����
	static inline unordered_map<PacketType, bool> _PacketWaitStatus; // �� PACKET ��û�� ���� ��ٸ��� �ִ��� ���¸� ���� (1 = ��ٸ�����, 0 = ��ٸ��� �ʴ���)
};