#include <windows.h>
#include<winsock.h>
#include <iostream>
#include <thread>
#include "UDPServer.h"

using namespace std;

int main(int argc, char** argv)
{
	UDPServer* server = new UDPServer();
	char ip[] = "127.0.0.1";
	server->connect(ip, 8001);
	while (1) {
		uint8_t msg[10];
		memset(msg, '\0', sizeof(msg));
		cin >> msg;
		server->send(msg, sizeof(msg));


		
	}
	server->disconnect();
}
