
#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <thread>
#include <deque>

using namespace std;

class UDPClient
{
public:
	UDPClient();
	~UDPClient();
	bool connect(char* serverIP, int port);
	void disconnect();
	int send(uint8_t* buff, int count);
	uint8_t readByte();
	void readAllBuffer(uint8_t result[]);
	int getBytesAvailableCount();

private:
	WSAData WinSocketData;

	SOCKET UdpClientSocket;
	struct sockaddr_in UdpServer;

	//receive parameters
	deque<uint8_t> rcvBuff;
	int UdpServerLen = sizeof(UdpServer);

	bool isConnected = false;
	void receiveLoop();
	thread receiveThread;

};