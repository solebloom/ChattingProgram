#pragma once
#include <boost/core/noncopyable.hpp>
#include "Session.cpp"

class ChatWriteAttribute : private boost::noncopyable {
public:
	virtual void WriteData(Session* session, size_t length) = 0;
};