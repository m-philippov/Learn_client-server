#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <winsock2.h>

#define PORT 9999

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;
int nSocket;
int nArrClient[5];

void ProcessNewMessage(int nClientSocket) {
	std::cout << "\n Processing the new message for client socket:" << nClientSocket;
	char buff[256 + 1] = { 0, };
	int nRet = recv(nClientSocket, buff, 256, 0);
	if (nRet < 0) {
		std::cout << "\n Something wrong happened..closing the connection for client";
		closesocket(nClientSocket);
		for (int nIndex = 0; nIndex < 5; nIndex++) {
			if (nArrClient[nIndex] == nClientSocket) {
				nArrClient[nIndex] = 0;
				break;
			}
		}
	}
	else {
		std::cout << "\n The message received from client is: " << buff;
		//Send the response to client
		send(nClientSocket, "Processed your request", 23, 0);
		std::cout << "\n**************************";
	}
}

void ProcessTheNewRequest() {
	//New connection request
	if (FD_ISSET(nSocket, &fr)) {
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen);
		if (nClientSocket > 0) {
			//Put into the client fd_set.
			int nIndex;
			for (nIndex = 0; nIndex < 5; nIndex++) {
				if (nArrClient[nIndex] == 0) {
					nArrClient[nIndex] = nClientSocket;
					send(nClientSocket, "Got the Connection done successfully", 37, 0);
					break;
				}
			}
			if (nIndex == 5) {
				std::cout << "\n No space for a new connection";
			}
		}

	}
	else {
		for (int nIndex = 0; nIndex < 5; nIndex++) {
			if (FD_ISSET(nArrClient[nIndex], &fr)) {
				//Got the new message from the client 
				//Just recv new message
				//Just queue that for new workes of your server to fulfill the request
				ProcessNewMessage(nArrClient[nIndex]);
			}
		}
	}
}

int main() {

	int nRet = 0;

	//initialize the WSA variables
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
		std::cout << "\nWSA Failed to initialized";
	}
	else {
		std::cout << "\nWSA initialized";
	}

	//initialize the socket
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0) {
		std::cout << "\nThe socket not opened";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "\nThe socket opened " << nSocket;
	}

	//initilize the enviroment fo sockaddr structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	memset(&(srv.sin_zero), 0, 8);

	//Blocking vs not-blocking socked 
	//optval = 0 means blocking and !=0 means non blocking
	// each socket is blocking by default
	/*
	u_long optval = 0;
	nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	if (nRet != 0) {
		std::cout << "\nioctlsocket call failed";
	}
	else {
		std::cout << "\nioctlsocket call passed";
	}
	*/
	
	//setsockopt
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);
	if (!nRet) {
		std::cout << "\n The setsockopt call successful";
	}
	else {
		std::cout << "\n The setsockopt call failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	//Bind the socket to the local port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0) {
		std::cout << "\nFail to bind to local port";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "\nSuccessfully bind to local port";
	}
	//Listen the request from client (queues the requests)
	nRet = listen(nSocket, 5);
	if (nRet < 0) {
		std::cout << "\nFail to start listen to local port";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "\nStarted listening to local port";
	}
	/*
	int n = 100;
	for (int nIndex = 0; nIndex < 64; nIndex++) { //64 - max
		FD_SET(n, &fr);
		n++;
	}
	std::cout << "\n " << fr.fd_count << "\n ";
	for (int nIndex = 0; nIndex < 64; nIndex++) {
		std::cout << fr.fd_array[nIndex] << " ";
	}
	*/
	//select();
	nMaxFd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	
	while (1) {

		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);
		std::cout << "\nBefore select call: " << fr.fd_count;

		//new data as old data
		for (int nIndex = 0; nIndex < 5; nIndex++) {
			if (nArrClient[nIndex] != 0) {
				FD_SET(nArrClient[nIndex], &fr);
				FD_SET(nArrClient[nIndex], &fe);
			}
		}

		//Keep waiting for the new requests and proceed as per the request
		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0) {
			//When someone connects or communicates with a message over a dedicated connection
			//Please remember that the socket listening to new clients
			//and then communicating with the same client later are not the same.
			//After connection, you get one more socket to communicate with client
			//You need to send/recv message over the network using that new socket
			std::cout << "\nData on port .... Processing now ... ";
			//break;
			//Process the request
			/*
			if (FD_ISSET(nSocket, &fe)) {
				std::cout << "\n There us an exception. Just get away from here";
			}
			else if (FD_ISSET(nSocket, &fw)) {
				std::cout << "\n ready to write something";
			}
			else if (FD_ISSET(nSocket, &fr)) {
				std::cout << "\n ready to read. Something new came up at the port";
				//Accept the new connection
			}
			*/
			ProcessTheNewRequest();
		}
		else if (nRet == 0) {
			//No connection or any communication request made or you can say that  of the socket descriptors are ready
			std::cout << "\n Mothing on port: " << PORT;

		}
		else {
			//It failed and your application should show some useful messsage
			std::cout << "\n Mothing on port";
			exit(EXIT_FAILURE);
		}

		std::cout << "\n After the select call: " << fr.fd_count;
		//Sleep(2000);
	}
	

	return 0;
}