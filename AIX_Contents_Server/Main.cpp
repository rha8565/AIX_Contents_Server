#pragma comment(lib, "ws2_32")

#include <thread>
#include "ServerClass.h"


int main() {

	unique_ptr<ServerClass> ServerClassInstance = make_unique<ServerClass>();
	thread& ServerWaitForClientThread = ServerClassInstance->GetServerTickThread();
	ServerWaitForClientThread.join();
	return 0;
}

