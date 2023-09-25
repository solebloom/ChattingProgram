#pragma once
#include <boost/noncopyable.hpp>
#include "Session.cpp"

class ChatWriteAttribute : private noncopyable {
public:
	virtual void WriteData(Session* session, size_t length) = 0;
};