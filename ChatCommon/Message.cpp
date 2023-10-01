#pragma once
#include "Message.hpp"

Message::Message() : messagePacket() {
	
};

Message::~Message() {
	// Initialize the messagePacket to clear its contents.
	// Note: Modifying memset method to initializing is done to ensure that memory beyond MESSAGE_MAX_LENGTH isn't accessed.
	// Bad Case => memset(&messagePacket, 0, MESSAGE_MAX_LENGTH);
	messagePacket = {};
}