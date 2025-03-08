#include "Client.h"
#include "MenuManager.h"

Client::Client(io_context& io_context, const string& host, const string& port)
	:_socket(io_context), _io_context(io_context), _work_guard(make_work_guard(_io_context))
		,_thread_pool(thread::hardware_concurrency()) // cpu 코어 개수만큼 스레드 풀 생성
{
	tcp::resolver resolver(io_context); // 호스트와 포트를 해결
	connect(_socket, resolver.resolve(host, port)); // 서버에 연결

	// io_context 실행을 위한 별도 스레드 풀 생성 (서버 요청에 대한 응답을 받을 때 비동기적으로 동작 가능)
	// io_context 내부 작업 큐에서 빈 스레드에게 작업을 분배(즉, 멀티스레드 처리 가능)
	// 클라이언트에서 동시에 여러개의 요청을 서버로 보내게 될 경우 멀티스레드로 극복 가능
	for (unsigned int i = 0; i < thread::hardware_concurrency(); ++i) {
		boost::asio::post(_thread_pool, [this]() {
			//cout << "Client io_context running on thread " << this_thread::get_id() << endl;
			_io_context.run();
		});
	}
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

	cout << "Sending packet with size: " << header.size << endl;

	// 서버에 패킷 전송
	// io_context.run()이 멀티스레드 환경에서 큐에 있는 작업들을 처리하고 있기 때문에, 추가적인 post 호출은 불필요할것
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

	// 1. 먼저 패킷 헤더를 읽음 (type + size)
	shared_ptr<PacketHeader> header = make_shared<PacketHeader>();
	async_read(_socket, buffer(header.get(), sizeof(PacketHeader)),
		[this, header](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// 응답 크기를 받은 후 그 크기만큼 응답 본문을 읽기
				cout << "Packet Type: " << header->type << ", Data Size: " << header->size << endl;

				// 2. 패킷 데이터 읽기 (response 사이즈에 맞게 읽기)
				shared_ptr<string> response = make_shared<string>(header->size, '\0');
				async_read(_socket, buffer(*response),
					[this, header, response](boost::system::error_code ec, size_t length)
					{
						if (!ec)
						{
							// 응답 출력
							cout << "[Response]" << endl << *response << endl;

							if (header->type == PACKET_CREATE)
							{
								// uid 저장을 위해 Util::setUID() 호출
								cout << "PACKET_CREATE process call!" << endl;
								Util::setUID(stoi(response->c_str()));
								cout << "uid saved: " << Util::getUID() << endl;

								return;
							}

							if (header->type == PACKET_CREATE_ERROR)
							{
								// 닉네임 입력 메뉴에서 닉네임을 다시 입력하도록 처리
								cout << "PACKET_CREATE_ERROR process call!" << endl;
								MenuCreateUserName* menuCreateUserName = dynamic_cast<MenuCreateUserName*>(MenuManager::getInstance().getMenu(MenuIndex::MAKE_USERNAME_MENU)); // 안전하게 다운캐스팅 (menuCreateUserName*이 아닐경우 nullptr 반환)
								if (!menuCreateUserName)
								{
									cout << "menuCreateUserName is null!" << endl;
								}
								else
								{
									menuCreateUserName->setErrorText("Try with a different name.");
								}

								return;
							}

							if (header->type == PACKET_READ_RANKING)
							{
								cout << "PACKET_READ_RANKING process call!" << endl;
								MenuRanking* menuRanking = dynamic_cast<MenuRanking*>(MenuManager::getInstance().getMenu(MenuIndex::RANKING_MENU)); // 안전하게 다운캐스팅 (해당 형식이 아닐경우 nullptr 반환)
								if (!menuRanking)
								{
									cout << "menuRanking is null!" << endl;
									return;
								}

								// 한줄씩 데이터를 읽어서 username, score 데이터를 parameter로 넘기기 
								stringstream ss(*response);
								string line;

								while (getline(ss, line))
								{
									stringstream lineStream(line);
									string name;
									string score;

									// space로 구분된 name, score 읽은 후 rankData에 삽입
									if (lineStream >> name >> score) 
										menuRanking->pushRankData(name, score); 
								}

								return;
							}

							if (header->type == PACKET_READ_MAX_CLEAR_STAGE)
							{
								cout << "PACKET_READ_STAGE process call!" << endl;
								// uid에 해당하는 가장 높이 클리어했던 stage 저장
								Game::getInstance().setPlayerCurrentClearStage(Game::getInstance().getUid(), stoi(response->c_str()));
								return;
							}
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
