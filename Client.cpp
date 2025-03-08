#include "Client.h"
#include "MenuManager.h"

Client::Client(io_context& io_context, const string& host, const string& port)
	:_socket(io_context), _io_context(io_context), _work_guard(make_work_guard(_io_context))
		,_thread_pool(thread::hardware_concurrency()) // cpu �ھ� ������ŭ ������ Ǯ ����
{
	tcp::resolver resolver(io_context); // ȣ��Ʈ�� ��Ʈ�� �ذ�
	connect(_socket, resolver.resolve(host, port)); // ������ ����

	// io_context ������ ���� ���� ������ Ǯ ���� (���� ��û�� ���� ������ ���� �� �񵿱������� ���� ����)
	// io_context ���� �۾� ť���� �� �����忡�� �۾��� �й�(��, ��Ƽ������ ó�� ����)
	// Ŭ���̾�Ʈ���� ���ÿ� �������� ��û�� ������ ������ �� ��� ��Ƽ������� �غ� ����
	for (unsigned int i = 0; i < thread::hardware_concurrency(); ++i) {
		boost::asio::post(_thread_pool, [this]() {
			//cout << "Client io_context running on thread " << this_thread::get_id() << endl;
			_io_context.run();
		});
	}
}

Client::~Client()
{
	_io_context.stop(); // io_context ����
	if (_io_thread.joinable())
		_io_thread.join(); // �����尡 ���������� ���
}

void Client::send_packet_async(PacketType type, const string& data)
{
	PacketHeader header;
	header.type = type; // ��Ŷ ���� ����
	header.size = data.size(); // ��Ŷ �ٵ� ũ�� ����

	// data�� shared_ptr�� ���μ� �����ϰ� ����
	shared_ptr<string> data_ptr = make_shared<string>(data);

	// ��Ŷ ����� �ٵ� ���ۿ� �߰�
	vector<const_buffer> buffers;
	buffers.push_back(buffer(&header, sizeof(header)));
	buffers.push_back(buffer(*data_ptr)); // shared_ptr���� ���� data�� �����ͼ� ���ۿ� �߰�

	cout << "Sending packet with size: " << header.size << endl;

	// ������ ��Ŷ ����
	// io_context.run()�� ��Ƽ������ ȯ�濡�� ť�� �ִ� �۾����� ó���ϰ� �ֱ� ������, �߰����� post ȣ���� ���ʿ��Ұ�
	async_write(_socket, buffers, 
		[this, data_ptr](boost::system::error_code ec, size_t length) // data_ptr�� ĸó�Ͽ� �񵿱� �۾� �߿� �����ϰ� ����
		{
			if (!ec)
			{
				cout << "[Send packet] : " << data_ptr->c_str() << endl;
				receive_response(); // ���� �ޱ�
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

	// 1. ���� ��Ŷ ����� ���� (type + size)
	shared_ptr<PacketHeader> header = make_shared<PacketHeader>();
	async_read(_socket, buffer(header.get(), sizeof(PacketHeader)),
		[this, header](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// ���� ũ�⸦ ���� �� �� ũ�⸸ŭ ���� ������ �б�
				cout << "Packet Type: " << header->type << ", Data Size: " << header->size << endl;

				// 2. ��Ŷ ������ �б� (response ����� �°� �б�)
				shared_ptr<string> response = make_shared<string>(header->size, '\0');
				async_read(_socket, buffer(*response),
					[this, header, response](boost::system::error_code ec, size_t length)
					{
						if (!ec)
						{
							// ���� ���
							cout << "[Response]" << endl << *response << endl;

							if (header->type == PACKET_CREATE)
							{
								// uid ������ ���� Util::setUID() ȣ��
								cout << "PACKET_CREATE process call!" << endl;
								Util::setUID(stoi(response->c_str()));
								cout << "uid saved: " << Util::getUID() << endl;

								return;
							}

							if (header->type == PACKET_CREATE_ERROR)
							{
								// �г��� �Է� �޴����� �г����� �ٽ� �Է��ϵ��� ó��
								cout << "PACKET_CREATE_ERROR process call!" << endl;
								MenuCreateUserName* menuCreateUserName = dynamic_cast<MenuCreateUserName*>(MenuManager::getInstance().getMenu(MenuIndex::MAKE_USERNAME_MENU)); // �����ϰ� �ٿ�ĳ���� (menuCreateUserName*�� �ƴҰ�� nullptr ��ȯ)
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
								MenuRanking* menuRanking = dynamic_cast<MenuRanking*>(MenuManager::getInstance().getMenu(MenuIndex::RANKING_MENU)); // �����ϰ� �ٿ�ĳ���� (�ش� ������ �ƴҰ�� nullptr ��ȯ)
								if (!menuRanking)
								{
									cout << "menuRanking is null!" << endl;
									return;
								}

								// ���پ� �����͸� �о username, score �����͸� parameter�� �ѱ�� 
								stringstream ss(*response);
								string line;

								while (getline(ss, line))
								{
									stringstream lineStream(line);
									string name;
									string score;

									// space�� ���е� name, score ���� �� rankData�� ����
									if (lineStream >> name >> score) 
										menuRanking->pushRankData(name, score); 
								}

								return;
							}

							if (header->type == PACKET_READ_MAX_CLEAR_STAGE)
							{
								cout << "PACKET_READ_STAGE process call!" << endl;
								// uid�� �ش��ϴ� ���� ���� Ŭ�����ߴ� stage ����
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
