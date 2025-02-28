#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Packet.h"
#include "Util.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class Client
{
public:
	Client(io_context& io_context, const string& host, const string& port); // 생성자: 서버에 연결
	~Client();
	void send_packet_async(PacketType type, const string& data); // 서버에 패킷을 비동기적으로 전송하고 응답을 받는 함수
	bool isConnected() const;

private:
	tcp::socket _socket; // 서버와 통신할 소켓
	//tcp::resolver _resolver; // 호스트와 포트를 resolve (DNS 조회를 통해 도메인 이름을 IP 주소로 resolve)
	void receive_response(); // 서버로부터 응답을 받는 함수
	io_context& _io_context; // 작업 큐 역할을 하며, 비동기 작업을 큐에 등록
	executor_work_guard<io_context::executor_type> _work_guard; // io_context가 종료되지 않도록 유지
	thread_pool _thread_pool; // io_context 실행을 위한 스레드 풀
	thread _io_thread; // io_context 실행을 위한 스레드
};