#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <WinSock2.h>
#include <memory>
#include <fstream>
#include "ClientInfo.h"

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
	void ClientListenerThread();
	thread& GetServerTickThread();
	

};