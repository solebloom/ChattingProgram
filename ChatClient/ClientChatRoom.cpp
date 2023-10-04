#pragma once
#include <iostream>
//#include <boost/asio.hpp>
#include <unordered_set>
#include "../ChatCommon/ChatRoom.cpp"

using namespace boost::asio;
using namespace boost::system;
using ip::tcp;

#ifdef USE_SPIN_LOCK
#undef USE_SPIN_LOCK
#endif

class ClientChatRoom final : public ChatRoom {
public:
	ClientChatRoom(int roomIndex) : ChatRoom(roomIndex) {

	}

	~ClientChatRoom() {

	}

protected:
	void AllowEnteringRoom(Session* session) final {
		session->SetRoomIndex(roomIndex);
	}
	void DenyEnteringRoom(Session* session) final {
		// ChatClient doesn't need to judge to block entering room. just allow it with believing ChatServer.
		ClearDisconnectedSession();

		AllowEnteringRoom(session);
	}
	void ProcessWritingData(Session* session, std::size_t length) final {
		// TODO: implements it.
	}
};