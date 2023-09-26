#pragma once
//#include <iostream>
//#include <boost/asio.hpp>
#include <unordered_map>
#include <atomic>
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
					std::cout << "클라이언트가 연결되었습니다." << std::endl;
					ReadData(new Session(std::move(socket)));
				} else {
					std::cout << "클라이언트가 연결에 실패했습니다." << std::endl;
				}
				AcceptClient(io_context);
			}
		);
	}

	void WriteData(Session* session) {
		auto messagePacket = session->GetMessagePacket();
		async_write(session->GetSocket(),
			buffer(&messagePacket, Message::MESSAGE_HEADER_MAX_LENGTH + messagePacket.messageHeader.bodyLength),
			[this](error_code ec, std::size_t length) {
				if (ec.failed()) {
					std::cout << "ChatServer::WriteData Error " << ec << " : " << ec.message() << std::endl;
				}
			}
		);
	}

	// TODO: Fix it using config file load system or CommonUtil class
	constexpr static char keyValueSeparator = *const_cast<char*>(":");
	constexpr static char dataSeparator = *const_cast<char*>(",");
	void WriteData(Session* session, std::size_t length) {
		// TODO: Data should be filtered by condition through enum MessageType and processed.
		bool isEnterRoom = session->IsEnteredRoom();
		int roomIndex = session->GetRoomIndex();

		auto messagePacket = session->GetMessagePacket();
		Message::MessageHeader header = messagePacket.messageHeader;
		header.userIndex = session->GetUserIndex();
		switch (header.messageType) {
		case Message::MessageType::CONNECT:
			// TODO 1-1: If will be added SERVER_INFO type in enum MessageType, this routine will be changed to process SERVER_INFO type.
			// TODO 1-2: And then, SERVER_ENTER type routine should send ROOM_INFO data.
			if (session->IsEnteredRoom()) {
				std::cout << "MessageType " << header.messageType << " can't allow sending in the chat room! uid : " << header.userIndex 
						<< ", roomIndex : " << session->GetRoomIndex() << std::endl;
				return;
			}

			header.messageType = Message::MessageType::ROOM_INFO;
			Message::MessageBody body = messagePacket.messageBody;
			memset(&body.data[0], 0, Message::MESSAGE_BODY_MAX_LENGTH);
			{
				int dataIndex = 0;
				for (auto it = chatRoomMap.begin(); it != chatRoomMap.end(); ++it) {
					int skipValue = sizeof(it->first);
					memcpy(&body.data[dataIndex], &it->first, skipValue);
					body.data[skipValue] = keyValueSeparator;
				
					int userCount = it->second->GetUserCount();
					int sizeofUserCount = sizeof(userCount);
					memcpy(&body.data[skipValue + 1], &userCount, sizeofUserCount);
					skipValue += sizeofUserCount + 1;
					body.data[skipValue] = dataSeparator;
				
					dataIndex += skipValue + 1;
				}
				header.bodyLength = dataIndex;
			}
			break;
		case Message::MessageType::DISCONNECT:
			// TODO: ChatRoom::WriteData function will process this routine.
			break;
		case Message::MessageType::ROOM_CREATE:
			if (isEnterRoom) {
				std::cout << "MessageType " << header.messageType << " can't allow sending in the chat room! uid : " << header.userIndex
					<< ", roomIndex : " << session->GetRoomIndex() << std::endl;
				return;
			} else {
				roomIndex = ++lastRoomIndex;
				ChatRoom* chatRoom = new ChatRoom{roomIndex};
				auto ret = chatRoomMap.insert(std::unordered_map<int, ChatRoom*>::value_type(roomIndex, chatRoom));
				if (!ret.second) {
					// Unexpected behavior! ChatRoom can't be created! this roomIndex is already used!
					delete chatRoom;
					
					roomIndex = ret.first->first;
					chatRoom = ret.first->second;
					
					header.messageType = Message::MessageType::ROOM_ENTER;
					
					Message::MessageBody body = messagePacket.messageBody;
					header.bodyLength = sizeof(roomIndex);
					memcpy(&body.data[0], &roomIndex, header.bodyLength);
				}

				chatRoom->EnterRoom(session);
			}
			break;
		case Message::MessageType::ROOM_ENTER:
			if (isEnterRoom) {
				// TODO: ChatRoom::WriteData function will process this routine.
				std::cout << "MessageType " << header.messageType << " can't allow sending in the chat room! uid : " << header.userIndex
					<< ", roomIndex : " << session->GetRoomIndex() << std::endl;
				return;
			} else {
				memcpy(&roomIndex, &messagePacket.messageBody.data, sizeof(roomIndex));
				session->SetRoomIndex(roomIndex);
			}
			break;
		case Message::MessageType::ROOM_EXIT:
			if (!isEnterRoom) {
				// TODO: ChatRoom::WriteData function will process this routine.
				std::cout << "MessageType " << header.messageType << " can't allow sending without joining a chat room! uid : " << header.userIndex
					<< ", roomIndex : " << session->GetRoomIndex() << std::endl;
				return;
			}
			break;
		case Message::MessageType::BROADCAST:
			break;
		default:
			std::cout << "Unsupported MessageType " << header.messageType << " is found! uid : " << header.userIndex << std::endl;
			return;
		}

		if (isEnterRoom) {
			auto it = chatRoomMap.find(roomIndex);
			if (it != chatRoomMap.end()) {
				it->second->WriteData(session, length);
			}
		} else {
			// no-op
		}

		ReadData(session);
	}

	void Clear() {
		acceptor.close();
	}

	io_context& io_context;
	tcp::acceptor acceptor;

	std::atomic<int> lastRoomIndex{0};
	std::unordered_map<int, ChatRoom*> chatRoomMap;
};