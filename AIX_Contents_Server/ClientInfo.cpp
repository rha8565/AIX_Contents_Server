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

bool ClientInfo::TryJoinClient()
{
	if (mNowConnectNum < mMaxConnectNum)
	{
		mNowConnectNum++;
		return true;
	}
	return false;
}

void ClientInfo::ClientExit()
{
	--mNowConnectNum;
}
