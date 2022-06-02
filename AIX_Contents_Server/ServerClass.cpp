#include "ServerClass.h"

ServerClass::ServerClass()
{
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(23000);
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ClientInfoMap CDKeyMap;

	CDKeyFileRead();
	RefreshDisplay();

	// reset winsock
	if (WSAStartup(MAKEWORD(2, 0), &mWSAData) != 0)
		return;

	// Make winsock
	mServerSock = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSock == INVALID_SOCKET)
	{
		cout << "Socket Error No : " << WSAGetLastError() << endl;
		return;
	}

	BOOL bValid = true;
	setsockopt(mServerSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&bValid, sizeof(bValid));

	if (bind(mServerSock, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) != 0)
	{
		cout << "Bind Error No : " << WSAGetLastError() << endl;
		return;
	}

	if (listen(mServerSock, 5) != 0) {
		cout << "Listen Error No : " << WSAGetLastError() << endl;
		return;
	}
	
	mServerTickThread = thread(&ServerClass::ServerTick, this);
}

ServerClass::~ServerClass()
{
	closesocket(mServerSock);
	WSACleanup();
}

void ServerClass::CDKeyFileRead()
{
	ifstream inFile("CDKey.txt"); // 파일 Read
	string strBuffer; // 파일 읽기 버퍼
	string strToIntBuffer; //정수변환 버퍼
	string CDKeyTemp;
	uint32_t dueDateTemp;
	uint16_t MaxConnectNumTemp;

	while (inFile.peek() != EOF) {
		getline(inFile, strBuffer);
		stringstream strStream(strBuffer);
		strStream >> CDKeyTemp;
		strStream >> strToIntBuffer;
		dueDateTemp = stoi(strToIntBuffer);
		strStream >> strToIntBuffer;
		MaxConnectNumTemp = stoi(strToIntBuffer);
		auto rTemp = mCDKeyMap.insert(pair<string, unique_ptr<ClientInfo>>(CDKeyTemp, make_unique<ClientInfo>(CDKeyTemp, dueDateTemp, MaxConnectNumTemp)));
		if (!(rTemp.second))
		{
			cout << "Duplicate CD key cannot exist!!!" << endl;
			std::this_thread::sleep_for(chrono::seconds(10));
			exit(0);
		}
	}
	inFile.close();
}

void ServerClass::RefreshDisplay()
{
	system("cls");
	cout << "[AIXLAB Game Server]" << endl << endl;
	for (auto& elem : mCDKeyMap) {
		cout << elem.second.get()->MakeAndGetDisplayData() << endl;
	}
	cout << endl << "[v1_3 ]" << endl;
}


void ServerClass::ServerTick()
{

	SOCKET clientSock;
	struct sockaddr_in ClientAddr;
	int addrLen = sizeof(ClientAddr);

	while (1) {
		// 소켓 접속 대기
		clientSock = accept(mServerSock, (struct sockaddr*)&ClientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			cout << "Accept Error No : " << WSAGetLastError() << endl;
			return;
		}
		thread clientListenThread(&ServerClass::ClientListenerThread, this, clientSock);
		clientListenThread.detach();
	}
}

void ServerClass::ClientListenerThread(SOCKET clientSock)
{
	SOCKET cSockTemp = clientSock;
	string CDKeyTemp;
	string command;
	char listenBuffer[64];

	while (1) {
		
		memset(listenBuffer, 0, sizeof(listenBuffer));
		
		if (recv(cSockTemp, listenBuffer, sizeof(listenBuffer), 0) > 0) {
			
			stringstream bufStream(listenBuffer);
			bufStream >> CDKeyTemp;
			bufStream >> command;

			static mutex sMutex;
			if (command == "Join") 
			{
				scoped_lock<mutex> lock(sMutex);
				auto mapIter = mCDKeyMap.find(CDKeyTemp);
				if (mapIter == mCDKeyMap.end())
				{
					//kick client
					char sendBuf[] = "Fail";
					send(cSockTemp, sendBuf, sizeof(sendBuf), 0);
				}else
				{
					if (mapIter->second->TryJoinClient())
					{
						//join client
						char sendBuf[] = "Success";
						send(cSockTemp, sendBuf, sizeof(sendBuf), 0);
						RefreshDisplay();
					}else
					{
						//kick client
						char sendBuf[] = "Fail";
						send(cSockTemp, sendBuf, sizeof(sendBuf), 0);
					}
				}
			}else if (command == "EXIT") 
			{
				scoped_lock<mutex> lock(sMutex);
				auto mapIter = mCDKeyMap.find(CDKeyTemp);
				if (mapIter == mCDKeyMap.end())
				{
					//bad case
					closesocket(cSockTemp);
					return;
				}else
				{
					mapIter->second->ClientExit();
					RefreshDisplay();
					closesocket(cSockTemp);
					return;
				}
			}
		}
	}
}

thread& ServerClass::GetServerTickThread()
{
	return mServerTickThread;
}
