#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

void ErrorHandling(char* message);

int main(int argc, char *argv[]) 
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock[2];
	SOCKADDR_IN servAddr, clntAddr[2];

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		ErrorHandling("command line args error");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));
	
	if ( bind(hServSock, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() server");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("hServSock() server");
	}

	printf("Start listening on port %s \n", argv[1]);
	for (int i = 0; i < 2; ++i) {
		szClntAddr = sizeof(clntAddr[i]);
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr);
		if (hClntSock[i] == INVALID_SOCKET)
			ErrorHandling("accept() error");

		printf("clntAddr is : 0x%x\n", clntAddr[i].sin_addr.s_addr);

		send(hClntSock[i], message, sizeof(message), 0);
		while (1) {
			char buff[30];
			int strLen = recv(hClntSock[i], buff, sizeof(buff) - 1, 0);
			if (strLen == -1) {
				ErrorHandling("read() error!");
			}
			printf("Message from server: %s \n", buff);
		}
		closesocket(hClntSock[i]);

	}
	closesocket(hServSock);

	if (WSACleanup() == SOCKET_ERROR) {
		ErrorHandling("WSACleanup() error!");
	}
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}

//(0000 0001) (0000 0000) (0000 0000) (0111 1111)