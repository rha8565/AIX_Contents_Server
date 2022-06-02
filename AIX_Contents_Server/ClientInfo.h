#pragma once
#include <stdint.h>
#include <string>

using namespace std;

class ClientInfo final {

private:
	const string mCDKey;
	const uint32_t mDueDate;
	uint16_t mNowConnectNum;
	const uint16_t mMaxConnectNum;

public:
	ClientInfo(const string cdKey, const uint32_t dueData, const uint16_t mMaxConnectNum);
	ClientInfo() = delete;
	~ClientInfo() = default;
	string MakeAndGetDisplayData();
	bool TryJoinClient();
	void ClientExit();

};