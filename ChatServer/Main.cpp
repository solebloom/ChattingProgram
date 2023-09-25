#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include "ChatServer.cpp"

using namespace boost::asio;

int main(int argc, char* argv[]) {
	try {
		io_context io_context;
		ChatServer server{io_context, static_cast<ip::port_type>(8080)};
		io_context.run();
	} catch (std::exception& e) {
		std::cerr << "¿¡·¯ : " << e.what() << std::endl;
		return -1;
	}

	return 0;
}