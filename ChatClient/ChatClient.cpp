#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include "../ChatCommon/ChatReadWriteAttribute.cpp"

using namespace std::chrono;
using namespace boost::asio;
using ip::tcp;

class ChatClient : public ChatReadWriteAttribute {
public:
	ChatClient(io_context& io_context, std::string&& address, std::string&& port) {
		ConnectServer(io_context, std::move(address), std::move(port));
	}
	ChatClient(io_context& io_context, tcp::resolver::query query) {
		ConnectServer(io_context, std::move(query));
	}

	~ChatClient() {
		Close();
	}

private:
	void ConnectServer(tcp::socket&& socket, ip::basic_resolver_results<tcp>&& endpoint) {
		async_connect(socket,
			endpoint,
			[this, &socket](error_code ec, tcp::endpoint) {
				if (!ec.failed()) {
					std::cout << "서버에 연결되었습니다." << std::endl;
					this->session = new Session(std::move(socket), system_clock::to_time_t(system_clock::now()));
					ReadData(session);
				} else {
					std::cout << "서버 연결에 실패했습니다." << std::endl;
					Close();
				}
			});
	}
	void ConnectServer(io_context& io_context, std::string&& address, std::string&& port) {
		ConnectServer(std::move(tcp::socket{ io_context }),
					std::move(tcp::resolver{ io_context }.resolve(std::move(address), std::move(port))));
	}
	void ConnectServer(io_context& io_context, tcp::resolver::query&& query) {
		ConnectServer(std::move(tcp::socket{ io_context }),
					std::move(tcp::resolver{ io_context }.resolve(std::move(query))));
	}

	void WriteData(Session* session, size_t length) override {
		// TODO: implements it.
	}

	void Close() {
		if (session != nullptr) {
			delete session;
		}
	}

	Session* session;
};