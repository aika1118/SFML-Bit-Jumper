#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Packet.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class Client
{
public:
	Client(io_context& io_context, const string& host, const string& port); // ������: ������ ����
	void send_packet_async(PacketType type, const string& data); // ������ ��Ŷ�� �񵿱������� �����ϰ� ������ �޴� �Լ�

private:
	tcp::socket _socket; // ������ ����� ����
	//tcp::resolver _resolver; // ȣ��Ʈ�� ��Ʈ�� resolve (DNS ��ȸ�� ���� ������ �̸��� IP �ּҷ� resolve)
	void receive_response(); // �����κ��� ������ �޴� �Լ�
};