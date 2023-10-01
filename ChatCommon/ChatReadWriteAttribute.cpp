#pragma once
//#include <iostream>
//#include <boost/asio.hpp>
#include "ChatWriteAttribute.cpp"
//#include "Session.cpp"

//using namespace boost::asio;
//using namespace boost::system;

class ChatReadWriteAttribute : virtual public ChatWriteAttribute {
protected:
	void ReadData(Session* session) {
		// Reads Header Data
		Message::MessageHeader header = session->GetMessagePacket().messageHeader;
		async_read(session->GetSocket(),
			buffer(&header, Message::MESSAGE_HEADER_MAX_LENGTH),
			[this, &session](error_code ec, std::size_t headerLength) {
				if (!ec.failed()) {
					//If reading Header is successful, read body.
					ReadBody(session, headerLength);
				}
			}
		);
		/* Test Code (async_read_some method reads data repeatly by a certain size until reaches buffer size...)
		session->getSocket().async_read_some(buffer(&header, Message::MESSAGE_HEADER_MAX_LENGTH),
											[this, &session](error_code ec, std::size_t headerLength) {
												if (!ec.failed()) {
													//If reading Header is successful, read body.
													ReadBody(session, headerLength);
												}
											});
		*/
	}

	void ReadBody(Session* session, size_t headerLength) {
		Message::MessageHeader messageHeader = session->GetMessagePacket().messageHeader;

		// If client alters userIndex, repair userIndex.
		messageHeader.userIndex = session->GetUserIndex();

		Message::MessageBody messageBody = session->GetMessagePacket().messageBody;
		// If Body Data exists, reads it.
		if (messageHeader.bodyLength > 0) {
			async_read(session->GetSocket(),
				buffer(&messageBody, static_cast<size_t>(messageHeader.bodyLength)),
				[this, &session, &headerLength](error_code ec, std::size_t bodyLength) {
					if (!ec.failed()) {
						//std::cout << "클라이언트로부터 메시지 수신 : " << session->getMessage() << std::endl;
						WriteData(session, headerLength + bodyLength);
					}
				}
			);
		} else {
			WriteData(session, headerLength);
		}
	}
};