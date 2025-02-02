#include "Session.h"
#include "Server.h"

Session::Session(tcp::socket socket, Server& server, thread_pool& pool)
	: _socket(move(socket)), _server(server), _pool(pool), _header(), _body()
{
}

void Session::start()
{
	do_read_header(); // ��Ŷ ����� �б� ����
}

void Session::do_read_header()
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr�� �ڱ� �ڽ��� ����
	// socket : �����͸� ���� ����, �����ʹ� buffer�� ������
	// buffer : �����͸� ������ ����
	// handler : �б� �۾��� �Ϸ�Ǹ� ȣ��Ǵ� �ݹ� �Լ�
	async_read(_socket, buffer(&_header, sizeof(_header)), 
		[this, self](boost::system::error_code ec, size_t length) 
		{
			if (!ec)
			{
				// ��Ŷ ũ�� �˻� (�ִ� 1024����Ʈ�� ����)
				if (_header.size > 1024)
				{
					cerr << "Invalid packet size !" << endl;
					return;
				}
				do_read_body(); // ����� ���� �� �ٵ� �б� ����
			}
			else
			{
				cerr << "Read header error !" << endl;
				return;
			}
		}
	);
}

void Session::do_read_body()
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr�� �ڱ� �ڽ��� ����
	_body.resize(_header.size); // �ٵ� ũ�⸸ŭ ���۸� Ȯ��
	async_read(_socket, buffer(_body),
		[this, self](boost::system::error_code ec, size_t length) 
		{
			if (!ec)
			{
				// �ٵ� ���� �� ��Ŷ ó��
				post(_pool, [this, self] {handle_packet();}); // ������ Ǯ�� �۾��� �߰�
			}
			else
			{
				cerr << "Read body error !" << endl;
				return;
			}
		}
	);
}

void Session::handle_packet()
{
	switch (_header.type)
	{
		case PACKET_READ:
			handle_read_packet(); // DB �б� ��û ó��
			break;
		case PACKET_WRITE:
			handle_write_packet(); // DB ���� ��û ó��
			break;
		case PACKET_SAVE:
			handle_save_packet(); // ���� ���̺� ��û ó��
			break;
		default:
			cerr << "Unknown packet type !" << endl;
			break;
	}
}

void Session::handle_read_packet()
{
	MySQL_Driver* driver = get_mysql_driver_instance();
	unique_ptr<Connection> con(driver->connect(DB_HOST, DB_USERNAME, DB_PASSWORD));
	con->setSchema(DB_SCHEMA); 

	string query = "SELECT * FROM Plans WHERE user_id = ?";
	unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));
	pstmt->setInt(1, stoi(_body)); // Ŭ���̾�Ʈ�κ��� ���� �����͸� ���ε�
	unique_ptr<ResultSet> res(pstmt->executeQuery()); // ���� ����

	string result;
	while (res->next())
	{
		result += res->getString("column_name") + "\n";
	}

	send_response(result); // Ŭ���̾�Ʈ���� ��� ����
}

void Session::handle_write_packet()
{
	MySQL_Driver* driver = get_mysql_driver_instance();
	unique_ptr<Connection> con(driver->connect(DB_HOST, DB_USERNAME, DB_PASSWORD));
	con->setSchema(DB_SCHEMA);

	string query = "INSERT INTO Plans (plan_name, user_id) VALUES (?, ?)";
	unique_ptr<PreparedStatement> pstmt(con->prepareStatement(query));

	vector<string> body = parseBody(_body);
	if (body.size() != 2)
	{
		cerr << "Invalid body !" << endl;
		return;
	}
	pstmt->setString(1, body[0]);
	pstmt->setInt(2, stoi(body[1]));
	pstmt->executeQuery(); // ���� ����

	send_response("Write successful"); // Ŭ���̾�Ʈ���� ���� �޼��� ����
}

void Session::handle_save_packet()
{
	return;
}

void Session::send_response(const string& response)
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr�� �ڱ� �ڽ��� ����
	async_write(_socket, buffer(response),
		[this, self](boost::system::error_code ec, size_t length) 
		{
			if (!ec)
			{
				// ���� ���� �� ���� ����
				_socket.close(); // ���� �ݱ�
				cout << "Session closed successfully" << endl;
			}
		}
	);

	// shared_ptr self�� ���� ī��Ʈ�� 0�� �� �� Session ��ü�� �ڵ����� �Ҹ�
}

vector<string> Session::parseBody(const string& _body)
{
	vector<string> result;
	istringstream ss(_body);
	string token;

	while (getline(ss, token, ','))
	{
		result.push_back(token);
	}

	return result;
}
