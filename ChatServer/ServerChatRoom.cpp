#pragma once
#include <iostream>
//#include <boost/asio.hpp>
#include <unordered_set>
#include "../ChatCommon/ChatRoom.cpp"

using namespace boost::asio;
using namespace boost::system;
using ip::tcp;

class ServerChatRoom : public ChatRoom {
public:
	ServerChatRoom(int roomIndex) : ChatRoom(roomIndex) {

	}

	~ServerChatRoom() {

	}

protected:
	void ProcessWritingData(Session* session, std::size_t length) final {
		if (sessionList.size() > 1) {
			auto messagePacket = session->GetMessagePacket();
			auto message = buffer(&messagePacket, length);
			for (Session* otherSession : sessionList) {
				if (otherSession->GetUserIndex() != session->GetUserIndex()) {
					async_write(otherSession->GetSocket(),
						message,
						[this, &otherSession](error_code ec, std::size_t length) {
							if (ec.failed()) {
								if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
									disconnectedSessionList.insert(otherSession);
								}
								else {
									std::cout << "WriteData Error " << ec << " : " << ec.message() << std::endl;
								}
							}
						}
					);
				}
			}
		}
	}
};