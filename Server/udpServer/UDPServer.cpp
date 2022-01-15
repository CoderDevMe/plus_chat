#include "UDPServer.h"
#include <string.h>

UDPServer::UDPServer()
{
	cout << "----------- UDP Server ---------------" << endl << endl;

	int startUpResult = WSAStartup(MAKEWORD(2, 2), &WinSocketData);
	if (startUpResult != 0) {
		cout << "WsaStartUp Failed" << endl;
	}
	cout << "WSAStartUp Success" << endl;
}

UDPServer::~UDPServer()
{
	isConnected = false;
	receiveThread.join();
	int closeSocketResult = closesocket(UdpServerSocket);
	if (closeSocketResult == SOCKET_ERROR) {
		cout << "Closing socket failed with Error No " << WSAGetLastError() << endl;
	}
	cout << "Socket Close success" << endl;


	int cleanUpResult = WSACleanup();
	if (cleanUpResult == SOCKET_ERROR) {
		cout << "WSA CleanUp failed with Error No " << WSAGetLastError() << endl;
	}
	cout << "WSA CleanUp success" << endl;

}

bool UDPServer::connect(char* serverIP, int port) {

	UdpClient.sin_family = AF_INET;
	UdpClient.sin_addr.s_addr = inet_addr(serverIP);
	UdpClient.sin_port = htons(port);

	//creat socket
	UdpServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UdpServerSocket == INVALID_SOCKET) {
		cout << "Socket creation failed with Error No. " << WSAGetLastError() << endl;
		isConnected = false;
		return false;
	}
	cout << "Socket creation success" << endl;

	//bind
	int bindResult = bind(UdpServerSocket, (const SOCKADDR*)&UdpClient, (int)sizeof(UdpClient));
	if (bindResult == SOCKET_ERROR) {
		cout << "Binding failed with Error No " << WSAGetLastError() << endl;
		return false;
	}
	cout << "binding succeeded" << endl;

	isConnected = true;

	rcvBuff.clear();
	receiveThread = thread(&UDPServer::receiveLoop, this);
	return true;
}

void UDPServer::receiveLoop() {
	while (isConnected) {
		char _buff[512];
		int _buffLen = sizeof(_buff) + 1;
		memset(_buff, NULL, sizeof(_buff));

		int receiveFromResult = recvfrom(UdpServerSocket, _buff, _buffLen, 0, (SOCKADDR*)&UdpClient, &UdpClientLen);
		if (receiveFromResult == SOCKET_ERROR && WSAGetLastError() != 10054) {
			cout << "receiving Error with Error No. " << WSAGetLastError() << endl;
			continue;
		}

		for (int i = 0; i < receiveFromResult; i++) {
			rcvBuff.push_back(_buff[i]);
		}
	}
}

uint8_t UDPServer::readByte() {
	if (rcvBuff.empty())
		return NULL;
	uint8_t& c = rcvBuff.front();
	rcvBuff.pop_front();
	return c;
}

void UDPServer::readAllBuffer(uint8_t result[]) {
	if (rcvBuff.empty())
		return;

	memset(result, '\0', sizeof(result));

	int n = rcvBuff.size();
	for (int i = 0; i < n; i++) {
		result[i] = rcvBuff.front();
		rcvBuff.pop_front();
	}
}

int UDPServer::getBytesAvailableCount() {
	return rcvBuff.size();
}

int UDPServer::send(uint8_t * buff, int count) {
	int sendToResult = sendto(UdpServerSocket, (char *)buff, count, MSG_DONTROUTE, (const sockaddr*)&UdpClient, sizeof(UdpClient));
	if (sendToResult == SOCKET_ERROR) {
		cout << "send Error" << endl;
		return 0;
	}
	memset(buff, '\0', sizeof(buff));
	cout << "send succeeded" << endl;
	return sendToResult;
}

void UDPServer::disconnect() {
	this->~UDPServer();
}