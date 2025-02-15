#include "Client.h"

Client::Client(io_context& io_context, const string& host, const string& port)
	:_socket(io_context), _io_context(io_context), _work_guard(make_work_guard(_io_context))
{
	tcp::resolver resolver(io_context); // 호스트와 포트를 해결
	connect(_socket, resolver.resolve(host, port)); // 서버에 연결

	// io_context 실행을 위한 별도 스레드 생성 (서버 요청에 대한 응답을 받을 때 비동기적으로 동작 가능)
	_io_thread = thread([this]() {
		cout << "Client io_context running..." << endl;
		_io_context.run();  // io_context 실행 (작업이 없더라도 종료되지 않도록 work_guard로 유지중)
	});
}

Client::~Client()
{
	_io_context.stop(); // io_context 종료
	if (_io_thread.joinable())
		_io_thread.join(); // 스레드가 끝날때까지 대기
}

void Client::send_packet_async(PacketType type, const string& data)
{
	PacketHeader header;
	header.type = type; // 패킷 종류 설정
	header.size = data.size(); // 패킷 바디 크기 설정

	// data를 shared_ptr로 감싸서 안전하게 관리
	shared_ptr<string> data_ptr = make_shared<string>(data);

	// 패킷 헤더와 바디를 버퍼에 추가
	vector<const_buffer> buffers;
	buffers.push_back(buffer(&header, sizeof(header)));
	buffers.push_back(buffer(*data_ptr)); // shared_ptr에서 실제 data를 가져와서 버퍼에 추가

	// 서버에 패킷 전송
	async_write(_socket, buffers, 
		[this, data_ptr](boost::system::error_code ec, size_t length) // data_ptr을 캡처하여 비동기 작업 중에 안전하게 유지
		{
			if (!ec)
			{
				cout << "[Send packet] : " << data_ptr->c_str() << endl;
				receive_response(); // 응답 받기
			}
			else
			{
				cerr << "send packet error !" << endl;
				return;
			}
		}
	);
}
bool Client::isConnected() const
{
	return _socket.is_open();
}
void Client::receive_response()
{
	cout << "[Receive response]" << endl;
	shared_ptr<uint32_t> response_size = make_shared<uint32_t>(); // 응답 크기를 저장할 변수
	async_read(_socket, buffer(response_size.get(), sizeof(uint32_t)),
		[this, response_size](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// 응답 크기를 받은 후 그 크기만큼 응답 본문을 읽기
				cout << "Response size: " << *response_size << endl;

				// 응답 크기에 맞는 크기의 버퍼 생성
				shared_ptr<string> response = make_shared<string>(*response_size, '\0');

				// 응답 본문 읽기
				async_read(_socket, buffer(*response),
					[this, response](boost::system::error_code ec, size_t length)
					{
						if (!ec)
						{
							// 응답 출력
							cout << "[Response]" << endl << *response << endl;
						}
						else
						{
							cerr << "Error reading response: " << ec.message() << endl;
						}
					}
				);
			}
			else
			{
				cerr << "Error reading response size: " << ec.message() << endl;
			}
		}
	
	);
}	
