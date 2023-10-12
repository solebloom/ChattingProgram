
class RoomInfo {
public:
	int GetRoomIndex() {
		return roomIndex;
	}
	void setRoomIndex(int roomIndex) {
		this->roomIndex = roomIndex;
	}
	int GetUserCount() {
		return userCount;
	}
	void setUserCount(int userCount) {
		this->userCount = userCount;
	}

private:
	int roomIndex;
	int userCount;
};