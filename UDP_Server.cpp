#include "iostream"
#include "winsock2.h"
#include "ws2tcpip.h"
using namespace std;
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib,"Ws2_32.lib")

int main(int argc, char* argv[]) {
	// Step1: Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)){
		cout << "Winsock 2.2 is not supported\n";
			return 0;
	}

	// Step 2: Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client == INVALID_SOCKET) {
		cout << "Error " << WSAGetLastError()<< ": Cannot create server socket.";
		return 0;
	}	
	cout << "Client started!\n";

	// (Optional) Set time-out for receiving
	int tv = 10000; // Time-out interval: 10000 ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	// Step 3: Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	// Step 4: Communicate with server
	char buff[BUFF_SIZE];
	int ret, serverAddrLen = sizeof(serverAddr), messageLen;
	while (1) {
		// Send message
		cout <<"send to server: ";
		gets_s(buff, BUFF_SIZE);
		messageLen = (int)strlen(buff);
		if (messageLen == 0) break;

		ret = sendto(client, buff, messageLen, 0,
			(sockaddr*)&serverAddr, serverAddrLen);
		if (ret == SOCKET_ERROR) {
			printf("Error %d: Cannot send message.", WSAGetLastError());
		}

		// Receive echo message 
		ret = recvfrom(client, buff, BUFF_SIZE, 0,
			(sockaddr*)&serverAddr, &serverAddrLen);

		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT)
				printf("Time-out!");
			else printf("Error %d: Cannot receive message.", WSAGetLastError());
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("Receive from server: %s\n", buff);
		}
	} // end while

	// Step 5: Close socket
	closesocket(client);

	// Step 6: Terminate Winsock
	WSACleanup();

	return 0;
}