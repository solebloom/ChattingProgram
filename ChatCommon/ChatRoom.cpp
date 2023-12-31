#pragma once
#include <iostream>
//#include <boost/asio.hpp>
#include <concurrent_unordered_set.h>
#include <unordered_set>
#include "ChatWriteAttribute.cpp"
#include "Message.hpp"
#include "SpinLock/CLHLock.hpp"
//#include "Session.cpp"

using namespace boost::asio;
using namespace boost::system;
using ip::tcp;

#define USE_SPIN_LOCK

class ChatRoom abstract : virtual public ChatWriteAttribute {
public:
	ChatRoom(int roomIndex) : roomIndex(roomIndex) {
		
	}

	virtual ~ChatRoom() {
		Clear();
	}

	int GetUserCount() {
		return static_cast<int>(sessionList.size());
	}

	void EnterRoom(Session* session) {
		if (GetUserCount() < MAX_USER_COUNT) {
			sessionList.insert(session);

			AllowEnteringRoom(session);
		} else {
			DenyEnteringRoom(session);
		}
	}
	void LeaveRoom(Session* session) {
		session->InitRoomIndex();

		disconnectedSessionList.insert(session);
	}

	virtual void WriteData(Session* session, std::size_t length) final {
#ifdef USE_SPIN_LOCK
		std::lock_guard<CLHLock> lock(spinLock);
#endif
		ClearDisconnectedSession();

		ProcessWritingData(session, length);
	}

protected:
	virtual void AllowEnteringRoom(Session* session) = 0;
	virtual void DenyEnteringRoom(Session* session) = 0;
	virtual void ProcessWritingData(Session* session, std::size_t length) = 0;
	
	void ClearDisconnectedSession() {
		for (Session* disconnectedSession : disconnectedSessionList) {
			sessionList.unsafe_erase(disconnectedSession);
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

	CLHLock spinLock{};

	int roomIndex;
	// TODO: Fix it using config file load system.
	constexpr static int MAX_USER_COUNT = 100;
	concurrency::concurrent_unordered_set<Session*> sessionList;
	std::unordered_set<Session*> disconnectedSessionList;
};