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
	using Callback = function<void(const string& response)>; // �ݹ��Լ��� ��� (���� ǥ�������� ȣȯ����)
	/**
	 * Ŭ���̾�Ʈ ��ü�� �����մϴ�.
	 *
	 * @param io_context I/O ���ؽ�Ʈ�Դϴ�.
	 * @param host ������ ȣ��Ʈ�Դϴ�.
	 * @param port ������ ��Ʈ�Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	Client(io_context& io_context, const string& host, const string& port); // ������: ������ ����
	/**
	 * Ŭ���̾�Ʈ ��ü�� �Ҹ��մϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	~Client();
	/**
	 * ������ ��Ŷ�� �񵿱������� �����ϰ� ������ �޴� �Լ��Դϴ�.
	 *
	 * @param type ��Ŷ�� Ÿ���Դϴ�.
	 * @param data ��Ŷ�� �������Դϴ�.
	 * @param cb �ݹ� �Լ��Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void send_packet_async(PacketType type, const string& data, Callback cb); // ������ ��Ŷ�� �񵿱������� �����ϰ� ������ �޴� �Լ�
	/**
	 * Ŭ���̾�Ʈ�� ������ ����Ǿ� �ִ��� Ȯ���մϴ�.
	 *
	 * @return Ŭ���̾�Ʈ�� ������ ����Ǿ� ������ true, �׷��� ������ false�Դϴ�.
	 *
	 * @throws ����
	 */
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
	uint32_t _request_counter = 0; // ��û ID ī����
	unordered_map<uint32_t, Callback> _callbacks; // ��û ID�� �ݹ� ����
	/**
	 * �����κ��� ������ �޴� �Լ��Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void receive_response(); // �����κ��� ������ �޴� �Լ�
};