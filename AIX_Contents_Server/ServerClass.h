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
#include "ClientInfo.h"
//#include <string>
//#include <ctime>
//#include <cstdio>   

using namespace std;
typedef unordered_map<string, unique_ptr<ClientInfo>> ClientInfoMap;

class ServerClass final {

private:
	ClientInfoMap mCDKeyMap;
	SOCKET mServerSock;
	WSADATA mWSAData;
	thread mServerTickThread;
public:
	ServerClass();
	~ServerClass();
	void CDKeyFileRead();
	void RefreshDisplay();
	void ServerTick();
	void ClientListenerThread(SOCKET clientSock);
	thread& GetServerTickThread();
	

};