#pragma once
//#include <iostream>
#include <boost/asio.hpp>
#include "CommonUtil.cpp"
#include "Message.hpp"

using namespace boost::asio;
using namespace boost::system;
using ip::tcp;

class Session {
public:
    Session(tcp::socket&& socket)
        : socket(std::move(socket)), roomIndex(INITIAL_INDEX), userIndex(INITIAL_INDEX), message(new Message()) {

    }
    // Move constructor
    Session(Session&& other) noexcept
        : socket(std::move(other.socket)), roomIndex(other.roomIndex), userIndex(other.userIndex), userName(other.userName), message(other.message) {

    }
    // Move assignment operator
    Session& operator=(Session&& other) noexcept {
        if (this != &other) {
            socket = std::move(other.socket);
            roomIndex = other.roomIndex;
            userIndex = other.userIndex;
            userName = other.userName;
            message = other.message;
        }
        return *this;
    }

    ~Session() {
        socket.close();

        if (message != nullptr) {
            delete message;
        }
    }
    
    constexpr tcp::socket& GetSocket() {
        return socket;
    }

    bool IsEnteredRoom() {
        return this->roomIndex > INITIAL_INDEX;
    }
    void SetRoomIndex(int roomIndex) {
        this->roomIndex = roomIndex;
    }
    void InitRoomIndex() {
        this->roomIndex = INITIAL_INDEX;
    }
    int GetRoomIndex() {
        return roomIndex;
    }

    void SetUserIndex(int userIndex) {
        this->userIndex = userIndex;
    }
    int GetUserIndex() {
        return userIndex;
    }

    void SetUserName(const char* userName) {
        this->userName = userName;
    }
    const char* GetUserName() const {
        return userName.c_str();
    }

    Message::MessageProtocol GetMessagePacket() {
        return message->messagePacket;
    }
    int8_t* GetMessageBody() {
        return message->messagePacket.messageBody.data;
    }

    //Supports hash function (because of unordered_set)
    size_t operator()(const Session& session) const {
        // Alternative Case => return std::hash<size_t>()((static_cast<size_t>(session.userIndex) << 32L) + static_cast<size_t>(session.roomIndex));
        return std::hash<int>()(CommonUtil::CantorPairing(session.userIndex, session.roomIndex));
    }
    //Supports operator== function (because of unordered_set)
    bool operator==(const Session& other) const {
        return userIndex == other.userIndex && roomIndex == other.roomIndex;
    }

private:
    constexpr static int INITIAL_INDEX = 0;

    tcp::socket socket;
    int roomIndex;
    int userIndex;
    std::string userName;

    Message* message;
};