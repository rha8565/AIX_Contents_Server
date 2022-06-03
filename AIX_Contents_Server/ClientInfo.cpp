#include "ClientInfo.h"

ClientInfo::ClientInfo(const string CDKey, const uint32_t dueData, const uint16_t MaxConnectNum)
	:mCDKey(CDKey), mDueDate(dueData), mNowConnectNum(0), mMaxConnectNum(MaxConnectNum)
{
}

string ClientInfo::MakeAndGetDisplayData()
{
	char cTemp[20];
	string displayData = "CDKey: ";
	memset(cTemp, ' ',sizeof(cTemp));
	strncpy(cTemp, mCDKey.c_str(), mCDKey.length());
	cTemp[19] = 0;
	displayData += cTemp;
	displayData += "     DueDate: ";
	displayData += to_string(mDueDate);
	displayData += "     NowConnect: ";
	displayData += to_string(mNowConnectNum);
	displayData += "     MaxConnect: ";
	displayData += to_string(mMaxConnectNum);

	return displayData;
}

int8_t ClientInfo::TryJoinClient(uint32_t accessDate)
{
	if (mNowConnectNum >= mMaxConnectNum)
		return 0;
	if (mDueDate <= accessDate)
		return -1;
	mNowConnectNum += 1;
	return mNowConnectNum;
}

uint16_t ClientInfo::GetNowConnectNum()
{
	return mNowConnectNum;
}

void ClientInfo::ClientExit()
{
	--mNowConnectNum;
}
