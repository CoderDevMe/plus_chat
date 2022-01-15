#include "UDPClient.h"

UDPClient::UDPClient()
{
	cout << "----------- UDP Client ---------------" << endl << endl;

	int startUpResult = WSAStartup(MAKEWORD(2, 2), &WinSocketData);
	if (startUpResult != 0) {
		cout << "WsaStartUp Failed" << endl;
	}
	cout << "WSAStartUp Success" << endl;
}

UDPClient::~UDPClient()
{
	isConnected = false;
	receiveThread.join();
	int closeSocketResult = closesocket(UdpClientSocket);
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

bool UDPClient::connect(char* serverIP, int port) {

	UdpServer.sin_family = AF_INET;
	UdpServer.sin_addr.s_addr = inet_addr(serverIP);
	UdpServer.sin_port = htons(port);

	//creat socket
	UdpClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (UdpClientSocket == INVALID_SOCKET) {
		cout << "Socket creation failed with Error No. " << WSAGetLastError() << endl;
		isConnected = false;
		return false;
	}
	cout << "Socket creation success" << endl;
	uint8_t startMsg[] = "start";
	send(startMsg, 5);
	isConnected = true;

	rcvBuff.clear();
	receiveThread = thread(&UDPClient::receiveLoop, this);
	return true;
}

void UDPClient::receiveLoop() {
	while (isConnected) {
		char _buff[512];
		int _buffLen = sizeof(_buff) + 1;
		memset(_buff, NULL, sizeof(_buff));

		int receiveFromResult = recvfrom(UdpClientSocket, _buff, _buffLen, 0, (SOCKADDR*)&UdpServer, &UdpServerLen);
		if (receiveFromResult == SOCKET_ERROR && WSAGetLastError() != 10054) {
			cout << "receiving Error with Error No. " << WSAGetLastError() << endl;
			this->~UDPClient();
			break;
		}

		for (int i = 0; i < receiveFromResult; i++) {
			rcvBuff.push_back(_buff[i]);
		}
	}
}

uint8_t UDPClient::readByte() {
	if (rcvBuff.empty())
		return NULL;
	uint8_t& c = rcvBuff.front();
	rcvBuff.pop_front();
	return c;
}

void UDPClient::readAllBuffer(uint8_t result[]) {
	if (rcvBuff.empty())
		return;

	memset(result, '\0', sizeof(result));

	int n = rcvBuff.size();
	for (int i = 0; i < n; i++) {
		result[i] = rcvBuff.front();
		rcvBuff.pop_front();
	}
}

int UDPClient::getBytesAvailableCount() {
	return (rcvBuff.size());
}

int UDPClient::send(uint8_t* buff, int count) {
	int sendToResult = sendto(UdpClientSocket,(char *) buff, count, MSG_DONTROUTE, (const sockaddr*)&UdpServer, sizeof(UdpServer));
	if (sendToResult == SOCKET_ERROR) {
		cout << "send Error" << endl;
		return 0;
	}
	memset(buff, NULL, sizeof(buff));
	cout << "send succeeded" << endl;
	return sendToResult;
}

void UDPClient::disconnect() {
	this->~UDPClient();
}