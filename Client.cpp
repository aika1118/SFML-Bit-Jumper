#include "Client.h"

Client::Client(io_context& io_context, const string& host, const string& port)
	:_socket(io_context), _io_context(io_context), _work_guard(make_work_guard(_io_context))
{
	tcp::resolver resolver(io_context); // ȣ��Ʈ�� ��Ʈ�� �ذ�
	connect(_socket, resolver.resolve(host, port)); // ������ ����

	// io_context ������ ���� ���� ������ ���� (���� ��û�� ���� ������ ���� �� �񵿱������� ���� ����)
	_io_thread = thread([this]() {
		cout << "Client io_context running..." << endl;
		_io_context.run();  // io_context ���� (�۾��� ������ ������� �ʵ��� work_guard�� ������)
	});
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

	// ������ ��Ŷ ����
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
	shared_ptr<uint32_t> response_size = make_shared<uint32_t>(); // ���� ũ�⸦ ������ ����
	async_read(_socket, buffer(response_size.get(), sizeof(uint32_t)),
		[this, response_size](boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// ���� ũ�⸦ ���� �� �� ũ�⸸ŭ ���� ������ �б�
				cout << "Response size: " << *response_size << endl;

				// ���� ũ�⿡ �´� ũ���� ���� ����
				shared_ptr<string> response = make_shared<string>(*response_size, '\0');

				// ���� ���� �б�
				async_read(_socket, buffer(*response),
					[this, response](boost::system::error_code ec, size_t length)
					{
						if (!ec)
						{
							// ���� ���
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
