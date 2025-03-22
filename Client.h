#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "Packet.h"
#include "Util.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class Client : public enable_shared_from_this<Client>
{
public:
	Client(io_context& io_context, const string& host, const string& port); // ������: ������ ����
	~Client();
	void send_packet_async(PacketType type, const string& data); // ������ ��Ŷ�� �񵿱������� �����ϰ� ������ �޴� �Լ�
	bool isConnected() const;

private:
	tcp::socket _socket; // ������ ����� ����
	//tcp::resolver _resolver; // ȣ��Ʈ�� ��Ʈ�� resolve (DNS ��ȸ�� ���� ������ �̸��� IP �ּҷ� resolve)
	io_context& _io_context; // �۾� ť ������ �ϸ�, �񵿱� �۾��� ť�� ���
	executor_work_guard<io_context::executor_type> _work_guard; // io_context�� ������� �ʵ��� ����
	thread_pool _thread_pool; // io_context ������ ���� ������ Ǯ
	thread _io_thread; // io_context ������ ���� ������
	strand<io_context::executor_type> _strand; // ���� ���� ����ȭ (���ÿ� �ϳ��� �������� async_write ��û �� �浹 ����)
	string _host;
	string _port;

	void reconnect(); // ���� �翬��
	void receive_response(); // �����κ��� ������ �޴� �Լ�
	void process_response(shared_ptr<boost::asio::streambuf> buffer, const PacketHeader& header); // ������� ������ ��ó��
};