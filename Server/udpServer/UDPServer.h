#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <deque>

using namespace std;

class UDPServer
{
public:
	UDPServer();
	~UDPServer();
	bool connect(char* serverIP, int port);
	void disconnect();
	int send(uint8_t* buff, int count);
	uint8_t readByte();
	void readAllBuffer(uint8_t result[]);
	int getBytesAvailableCount();

private:
	WSAData WinSocketData;

	SOCKET UdpServerSocket;
	struct sockaddr_in UdpClient;

	//receive parameters
	deque<uint8_t> rcvBuff;
	int UdpClientLen = sizeof(UdpClient);	//this parameter used to initialize server, then used to store the connectd client

	bool isConnected = false;
	void receiveLoop();
	thread receiveThread;
};