#pragma once
//#include <iostream>
//#include <boost/asio.hpp>
#include <unordered_map>
#include "ChatReadWriteAttribute.cpp"
#include "ChatRoom.cpp"
//#include "Message.hpp"
//#include "Session.cpp"

//using namespace boost::asio;
//using namespace boost::system;
//using ip::tcp;

class ChatServer : public ChatReadWriteAttribute {
public:
	ChatServer(io_context& io_context, ip::port_type port)
		: io_context(io_context),
		acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
		AcceptClient(io_context);
	}

	~ChatServer() {
		Clear();
	}
	
private:
	void AcceptClient(io_context& io_context) {
		acceptor.async_accept(
			[this, &io_context](error_code ec, tcp::socket socket) {
				if (!ec.failed()) {
					std::cout << "Ŭ���̾�Ʈ�� ����Ǿ����ϴ�." << std::endl;
					ReadData(new Session(std::move(socket)));
				} else {
					std::cout << "Ŭ���̾�Ʈ�� ���ῡ �����߽��ϴ�." << std::endl;
				}
				AcceptClient(io_context);
			}
		);
	}

	void WriteData(Session* session, std::size_t length) {
		if (session->IsEnteredRoom()) {
			auto it = chatRoomMap.find(session->GetRoomIndex());
			if (it != chatRoomMap.end()) {
				it->second.WriteData(session, length);
			}
		} else {
			// TODO: Data should be filtered by condition through enum MessageType and processed.
		}

		ReadData(session);
	}

	void Clear() {
		acceptor.close();
	}

	io_context& io_context;
	tcp::acceptor acceptor;

	std::unordered_map<int, ChatRoom> chatRoomMap;
};