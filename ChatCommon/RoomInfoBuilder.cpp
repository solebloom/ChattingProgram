#pragma once
#include <type_traits>
#include <memory>
#include <cassert>
#include "RoomInfo.cpp"

class RoomInfoBuilder {
public:
	RoomInfoBuilder() : roomInfo(nullptr), settedVariableCount(0){

	}

	void SetVariable(int variable) {
		switch (++settedVariableCount) {
		case 1:
			roomInfo = std::make_unique<RoomInfo>();
			roomInfo->setRoomIndex(variable);
			break;
		case 2:
			roomInfo->setUserCount(variable);
			break;
		default:
			assert(settedVariableCount > 2 && "RoomInfoBuilder::settedVariableCount can't higher than 2.");
			break;
		}
	}

	std::unique_ptr<RoomInfo> Build() {
		return std::move(roomInfo);
	}

private:
	std::unique_ptr<RoomInfo> roomInfo;
	int settedVariableCount;
};