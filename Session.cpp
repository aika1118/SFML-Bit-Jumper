#include "Session.h"
#include "Server.h"

Session::Session(tcp::socket socket, Server& server, thread_pool& pool)
	: _socket(move(socket)), _server(server), _pool(pool), _header(), _body()
{
}

void Session::start()
{
	do_read_header(); // 패킷 헤더를 읽기 시작
}

void Session::do_read_header()
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr로 자기 자신을 유지
	// socket : 데이터를 읽을 소켓, 데이터는 buffer에 저장함
	// buffer : 데이터를 저장할 버퍼
	// handler : 읽기 작업이 완료되면 호출되는 콜백 함수
	async_read(_socket, buffer(&_header, sizeof(_header)), 
		[this, self](boost::system::error_code ec, size_t length) 
		{
			if (!ec)
			{
				// 패킷 크기 검사 (최대 1024바이트로 제한)
				if (_header.size > 1024)
				{
					cerr << "Invalid packet size !" << endl;
					return;
				}
				do_read_body(); // 헤더를 읽은 후 바디를 읽기 시작
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
	shared_ptr<Session> self(shared_from_this()); // shared_ptr로 자기 자신을 유지
	_body.resize(_header.size); // 바디 크기만큼 버퍼를 확장
	async_read(_socket, buffer(_body),
		[this, self](boost::system::error_code ec, size_t length) 
		{
			if (!ec)
			{
				// 바디를 읽은 후 패킷 처리
				post(_pool, [this, self] {handle_packet();}); // 스레드 풀에 작업을 추가
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
			handle_read_packet(); // DB 읽기 요청 처리
			break;
		case PACKET_WRITE:
			handle_write_packet(); // DB 쓰기 요청 처리
			break;
		case PACKET_SAVE:
			handle_save_packet(); // 게임 세이브 요청 처리
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
	pstmt->setInt(1, stoi(_body)); // 클라이언트로부터 받은 데이터를 바인딩
	unique_ptr<ResultSet> res(pstmt->executeQuery()); // 쿼리 실행

	string result;
	while (res->next())
	{
		result += res->getString("column_name") + "\n";
	}

	send_response(result); // 클라이언트에게 결과 전송
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
	pstmt->executeQuery(); // 쿼리 실행

	send_response("Write successful"); // 클라이언트에게 성공 메세지 전송
}

void Session::handle_save_packet()
{
	return;
}

void Session::send_response(const string& response)
{
	shared_ptr<Session> self(shared_from_this()); // shared_ptr로 자기 자신을 유지
	async_write(_socket, buffer(response),
		[this, self](boost::system::error_code ec, size_t length) 
		{
			if (!ec)
			{
				// 응답 전송 후 세션 종료
				_socket.close(); // 소켓 닫기
				cout << "Session closed successfully" << endl;
			}
		}
	);

	// shared_ptr self의 참조 카운트가 0이 될 때 Session 객체는 자동으로 소멸
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
