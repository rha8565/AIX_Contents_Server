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
	string FBuffer; // 파일 읽기 버퍼
	string IntBuffer; //정수변환 버퍼
	string CDKeyTemp;
	uint32_t dueDateTemp;
	uint16_t MaxConnectNumTemp;

	while (inFile.peek() != EOF) {
		getline(inFile, FBuffer);
		stringstream SBuffer(FBuffer);
		SBuffer >> CDKeyTemp;
		SBuffer >> IntBuffer;
		dueDateTemp = stoi(IntBuffer);
		SBuffer >> IntBuffer;
		MaxConnectNumTemp = stoi(IntBuffer);
		mCDKeyMap[CDKeyTemp] = make_unique<ClientInfo>(CDKeyTemp, dueDateTemp, MaxConnectNumTemp);
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
		thread clientListenThread(&ServerClass::ClientListenerThread, this);
		clientListenThread.detach();
	}
}

void ServerClass::ClientListenerThread()
{
	string CDKeyTemp;
	string command;
	while (1) {
		char listenBuffer[32];
		memset(listenBuffer, 0, sizeof(listenBuffer));
		if (recv(mServerSock, listenBuffer, sizeof(listenBuffer), 0) > 0) {
			cout << listenBuffer << endl;
			stringstream bufStream(listenBuffer);
			bufStream >> CDKeyTemp;
			bufStream >> command;
			if (command == "Join") {

			}
			else if (command == "EXIT") {

			}
			/*if (strcmp(listenBuffer, "Shutdown") == 0) {
				cout << "Close Socket" << endl;
				closesocket(sockTemp);
				return;
			}*/
		}
	}
}

thread& ServerClass::GetServerTickThread()
{
	return mServerTickThread;
}


void ServerClass() {

}
