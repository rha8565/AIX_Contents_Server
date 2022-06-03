#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <WinSock2.h>
#include <memory>
#include <fstream>
#include <mutex>
#include <assert.h> 
#include <chrono>
#include <ctime>
#include "ClientInfo.h"
//#include <cstdio>   

using namespace std;
typedef unordered_map<string, unique_ptr<ClientInfo>> ClientInfoMap;

class ServerClass final {

private:
	ClientInfoMap mCDKeyMap;
	SOCKET mServerSock;
	WSADATA mWSAData;
	thread mServerTickThread;
	ofstream Database;

public:
	ServerClass();
	~ServerClass();
	void CDKeyFileRead();
	void RefreshDisplay();
	void ServerTick();
	void ClientListenerThread(SOCKET clientSock);
	uint32_t GetNowDate();
	void WriteDatabase(string cdkey, uint32_t date,uint16_t userNum, uint8_t type);
	thread& GetServerTickThread();
	

};