#pragma comment(lib, "ws2_32")
#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <string>
#include <thread>    
#include <unordered_map>
#include <sstream>
#include <stdint.h>
#include <sstream>
#include "ServerClass.h"
//#include <string>
//#include <ctime>
//#include <cstdio>    
//#include <chrono>
//#include <mutex>

using namespace std;

void ClientListenerThread(SOCKET clientSock);

int main() {

	unique_ptr<ServerClass> ServerClassInstance = make_unique<ServerClass>();
	thread& ServerWaitForClientThread = ServerClassInstance->GetServerTickThread();
	ServerWaitForClientThread.join();
	return 0;
}

void ClientListenerThread(SOCKET clientSock) {
	SOCKET sockTemp = clientSock;
	string CDKeyTemp;
	string command;
	while (1) {
		char listenBuffer[32];
		memset(listenBuffer, 0, sizeof(listenBuffer));
		if (recv(sockTemp, listenBuffer, sizeof(listenBuffer), 0) > 0) {
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
