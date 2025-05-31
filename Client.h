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
	using Callback = function<void(const string& response)>; // 콜백함수로 사용 (람다 표현식으로 호환가능)
	/**
	 * 클라이언트 객체를 생성합니다.
	 *
	 * @param io_context I/O 컨텍스트입니다.
	 * @param host 서버의 호스트입니다.
	 * @param port 서버의 포트입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	Client(io_context& io_context, const string& host, const string& port); // 생성자: 서버에 연결
	/**
	 * 클라이언트 객체를 소멸합니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	~Client();
	/**
	 * 서버에 패킷을 비동기적으로 전송하고 응답을 받는 함수입니다.
	 *
	 * @param type 패킷의 타입입니다.
	 * @param data 패킷의 데이터입니다.
	 * @param cb 콜백 함수입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void send_packet_async(PacketType type, const string& data, Callback cb); // 서버에 패킷을 비동기적으로 전송하고 응답을 받는 함수
	/**
	 * 클라이언트가 서버와 연결되어 있는지 확인합니다.
	 *
	 * @return 클라이언트가 서버와 연결되어 있으면 true, 그렇지 않으면 false입니다.
	 *
	 * @throws 없음
	 */
	bool isConnected() const;

private:
	tcp::socket _socket; // 서버와 통신할 소켓
	//tcp::resolver _resolver; // 호스트와 포트를 resolve (DNS 조회를 통해 도메인 이름을 IP 주소로 resolve)
	io_context& _io_context; // 작업 큐 역할을 하며, 비동기 작업을 큐에 등록
	executor_work_guard<io_context::executor_type> _work_guard; // io_context가 종료되지 않도록 유지
	thread_pool _thread_pool; // io_context 실행을 위한 스레드 풀
	thread _io_thread; // io_context 실행을 위한 스레드
	strand<io_context::executor_type> _strand; // 소켓 접근 순차화 (동시에 하나의 소켓으로 async_write 요청 시 충돌 방지)
	string _host;
	string _port;
	uint32_t _request_counter = 0; // 요청 ID 카운터
	unordered_map<uint32_t, Callback> _callbacks; // 요청 ID와 콜백 매핑
	/**
	 * 서버로부터 응답을 받는 함수입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void receive_response(); // 서버로부터 응답을 받는 함수
};