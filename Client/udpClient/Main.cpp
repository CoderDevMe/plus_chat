#include <windows.h>
#include<winsock.h>
#include <iostream>
#include <thread>
#include "UDPClient.h"

using namespace std;

int main(int argc, char** argv)

{
	UDPClient* client = new UDPClient();
	char ip[] = "127.0.0.1";
	client->connect(ip, 8001);
	while (1) {
		if (client->getBytesAvailableCount() > 0) {
			cout << "received >> ";
			uint8_t rcdata[100];
			client->readAllBuffer(rcdata);
			for (int i = 0; i < 5; i++) {
				cout << (char)rcdata[i];
			}
			cout << endl;
		}
	}
}
