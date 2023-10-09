#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include "ChatClient.cpp"

using namespace boost::asio;

int main(int argc, char* argv[]) {
	try {
		io_context io_context;
		// TODO: Additional handling with taking arguments or GUI form is required.
		ChatClient client{ io_context, std::move(std::string("localhost")), std::move(std::string("8080")) };
		io_context.run();
	} catch (std::exception& e) {
		std::cerr << "¿¡·¯ : " << e.what() << std::endl;
		return -1;
	}

	return 0;
}