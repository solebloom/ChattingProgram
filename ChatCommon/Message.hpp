#pragma once
#include <cstdint>

class Message {
public:
	enum MessageType {
		NONE = 0,			// Default MessageType value, only use in constructor
		CONNECT,			// Client requests accepting connection to server
		DISCONNECT,			// Client requests disconnecting to server
//		SERVER_INFO,		// Server responses info about all of server (server number, current user count, etc.)
//		SERVER_ENTER,		// Client requests entering the server selected
//		SERVER_EXIT,		// Client requests exiting the server entered
		ALL_ROOM_INFO,		// Server responses info about all of room (room number, current user count, etc.)
		ROOM_CREATE,		// Client requests creating a chat room
		ROOM_ENTER,			// Client requests entering the room selected (Body Data includes info about joined user index, etc.)
		ROOM_EXIT,			// Client requests exiting the room entered
//		PREV_CHAT,			// Server responses previous sent chats
		BROADCAST,			// Client requests broadcasting my chat
//		UNICAST				// Client requests unicasting my chat to someone
	};

	#pragma pack(push, 1)
	struct MessageHeader {
		int32_t userIndex;			// sender UID
		MessageType messageType;	// it shows what client wants
		int32_t bodyLength;			// MessageBody::data variable's length
	};
	#pragma pack(pop)
	
	constexpr static int32_t MESSAGE_BODY_MAX_LENGTH = 1024;

	// TODO: Fix it using config file load system
	constexpr static char KEY_VALUE_SEPARATOR = *const_cast<char*>(":");
	// TODO: Fix it using config file load system
	constexpr static char DATA_SEPARATOR = *const_cast<char*>(",");

	#pragma pack(push, 1)
	struct MessageBody {
		int8_t data[MESSAGE_BODY_MAX_LENGTH];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	// Protocol usually has Header and Body
	struct MessageProtocol {
		MessageHeader messageHeader;
		MessageBody messageBody;
	} messagePacket; // Packet implements Protocol
	#pragma pack(pop)

	constexpr static int32_t MESSAGE_HEADER_MAX_LENGTH = sizeof(MessageProtocol::messageHeader);
	
	Message();
	~Message();
	
	constexpr static size_t MESSAGE_MAX_LENGTH = sizeof(MessageProtocol);
};