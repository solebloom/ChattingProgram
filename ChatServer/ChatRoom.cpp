#pragma once
#include <iostream>
//#include <boost/asio.hpp>
#include <unordered_set>
#include "ChatWriteAttribute.cpp"
#include "Message.hpp"
//#include "Session.cpp"

using namespace boost::asio;
using namespace boost::system;
using ip::tcp;

class ChatRoom : virtual public ChatWriteAttribute {
public:
	ChatRoom(int roomIndex) : roomIndex(roomIndex) {
		
	}

	~ChatRoom() {
		Clear();
	}

	void EnterRoom(Session* session) {
		if (sessionList.size() < MAX_USER_COUNT) {
			session->SetRoomIndex(roomIndex);

			sessionList.insert(session);
		} else {
			// TODO: Entrance Deny Notification should be sent unicast.
			//		 EnterRoom function's return value will be changed to bool or EnterRoom function will be processed that notification.
		}
	}
	void LeaveRoom(Session* session) {
		session->InitRoomIndex();

		disconnectedSessionList.insert(session);
	}

	void WriteData(Session* session, std::size_t length) {
		ClearDisconnectedSession();

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
								} else {
									std::cout << "WriteData Error " << ec << " : " << ec.message() << std::endl;
								}
							}
						}
					);
				}
			}
		}
	}

private:
	void ClearDisconnectedSession() {
		for (Session* disconnectedSession : disconnectedSessionList) {
			sessionList.erase(disconnectedSession);
			delete disconnectedSession;
		}
		disconnectedSessionList.clear();
	}

	void Clear() {
		ClearDisconnectedSession();

		for (Session* session : sessionList) {
			delete session;
		}
		sessionList.clear();
	}

	int roomIndex;
	// TODO: Fix it using config file load system.
	constexpr static int MAX_USER_COUNT = 100;
	std::unordered_set<Session*> sessionList;
	std::unordered_set<Session*> disconnectedSessionList;
};