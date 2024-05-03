#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>

#define PORT 9999

int nClientSocket;
struct sockaddr_in srv;



int main() {

	int nRet = 0;

	//initialize the WSA variables
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		std::cout << "\nWSAStartup failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	

	//initialize the socket
	int nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nClientSocket < 0) {
		std::cout << "\nThe socket call failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	

	//initilize the enviroment fo sockaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	//InetPton(AF_INET, _T("192.168.1.1"), &srv.sin_addr.s_addr);
	memset(&(srv.sin_zero), 0, 8);

	
	//Bind the socket to the local port
	nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nRet < 0) {
		std::cout << "\nconnect failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "\nconnected to the client";
		//Talk to the client ..
		/*
		while (1)
		*/
		char buff[255] = { 0, };
		recv(nClientSocket, buff, 255, 0);
		std::cout << "\n Just press any key to see the message received from the server";
		getchar();
		std::cout << "\n " << buff;
		while (1) {
			fgets(buff, 256, stdin);
			send(nClientSocket, buff, 256, 0);
			std::cout << "\nPress any key to get the response from server..";
			getchar();
			recv(nClientSocket, buff, 256, 0);
			std::cout << "\nNow send next message: ";
		}
	}
	
	return 0;
}