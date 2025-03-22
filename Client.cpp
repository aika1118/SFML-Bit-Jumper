#include "Client.h"
#include "MenuManager.h"

Client::Client(io_context& io_context, const string& host, const string& port)
	:_socket(io_context),
	_io_context(io_context),
	_strand(make_strand(io_context)), // _socket에 대한 비동기 작업을 순차적으로 실행하도록 보장
	_work_guard(make_work_guard(_io_context)),
	_thread_pool(thread::hardware_concurrency()) // cpu 코어 개수만큼 스레드 풀 생성
{
	tcp::resolver resolver(io_context); // 호스트와 포트를 해결

	connect(_socket, resolver.resolve(host, port)); // 서버에 연결

	// io_context 실행을 위한 별도 스레드 풀 생성 (서버 요청에 대한 응답을 받을 때 비동기적으로 동작 가능)
	// io_context 내부 작업 큐에서 빈 스레드에게 작업을 분배(즉, 멀티스레드 처리 가능)
	// 클라이언트에서 동시에 여러개의 요청을 서버로 보내게 될 경우 멀티스레드로 극복 가능
	// 멀티스레드에서 io_context 실행, 예외 처리 추가
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i) {
		boost::asio::post(_thread_pool, [this]() {
			try {
				//std::cout << "Thread " << std::this_thread::get_id() << " running io_context" << std::endl;
				_io_context.run();
			}
			catch (const std::bad_alloc& e) {
				std::cerr << "Bad allocation in io_context.run(): " << e.what() << std::endl;
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Invalid argument in io_context.run(): " << e.what() << std::endl;
			}
			catch (const std::exception& e) {
				std::cerr << "Exception in io_context.run(): " << e.what() << std::endl;
			}
			catch (...) {
				std::cerr << "Unknown exception in io_context.run()" << std::endl;
			}
			});
	}
}

Client::~Client()
{
	_io_context.stop(); // io_context 종료
	if (_io_thread.joinable())
		_io_thread.join(); // 스레드가 끝날때까지 대기
}

void Client::send_packet_async(PacketType type, const string& data, Callback cb = nullptr)
{
	// strand를 통해 비동기 작업을 순차화
	post(_strand, [this, type, data, cb]() {
		uint32_t request_id = _request_counter++; // 패킷 요청에 대한 고유 ID 생성
		if (cb) _callbacks[request_id] = cb; // 콜백 저장

		PacketHeader header;
		header.type = type; // 패킷 종류 설정
		header.size = data.size(); // 패킷 바디 크기 설정
		header.request_id = request_id; // 패킷 요청에 대한 고유 ID 

		// data를 shared_ptr로 감싸서 안전하게 관리
		shared_ptr<string> data_ptr = make_shared<string>(data);

		// 패킷 헤더와 바디를 버퍼에 추가
		vector<const_buffer> buffers;
		buffers.push_back(buffer(&header, sizeof(header)));
		buffers.push_back(buffer(*data_ptr)); // shared_ptr에서 실제 data를 가져와서 버퍼에 추가

		cout << "Sending packet with size: " << header.size << endl;

		// 서버에 패킷 전송 (strand로 콜백 바인딩)
		async_write(_socket, buffers,
			bind_executor(_strand,
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
			));
		});
}
bool Client::isConnected() const
{
	return _socket.is_open();
}
void Client::receive_response()
{
	cout << "[Receive response]" << endl;

	// streambuf를 사용해 헤더, response 한 번에 읽기
	shared_ptr<boost::asio::streambuf> buffer = make_shared<boost::asio::streambuf>();
	async_read(_socket, *buffer, boost::asio::transfer_at_least(sizeof(PacketHeader)),
		bind_executor(_strand,
			[this, buffer](boost::system::error_code ec, size_t length)
			{
				if (!ec && length >= sizeof(PacketHeader))
				{
					// streambuf에서 헤더 읽기
					const char* raw_data = static_cast<const char*>(buffer->data().data());
					shared_ptr<PacketHeader> header = make_shared<PacketHeader>();
					std::memcpy(header.get(), raw_data, sizeof(PacketHeader));

					cout << "Packet Type: " << header->type << ", Data Size: " << header->size << ", Request ID: " << header->request_id << endl;

					shared_ptr<string> response = make_shared<string>(raw_data + sizeof(PacketHeader), header->size); // response 데이터의 시작점으로 부터 header->size 만큼 response 읽기
					buffer->consume(sizeof(PacketHeader) + header->size); // streambuf에서 데이터를 읽어왔으니 해당 데이터를 버퍼에서 제거

					// 응답 출력
					cout << "[Response]" << endl << *response << endl;

					auto it = _callbacks.find(header->request_id);
					if (it != _callbacks.end())
					{
						it->second(response->c_str()); // 응답 데이터 전달하며 콜백 실행
						_callbacks.erase(it); // 완료 후 콜백 제거
					}
				}
				else
				{
					cerr << "Error reading response size: " << ec.message() << endl;
				}
			}
		));
}
