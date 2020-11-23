#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Msg {
	int dx;
	int dy;
	int isJ;//isJ is bool type 0 1
} Msg;

void ErrorHandling(char* message);
char* GetCharArray(char* buf, Msg msg); 


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;
	if (argc != 3)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket,(SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
	}

	strLen = recv(hSocket, message, sizeof(message)-1, 0);
	if (strLen == -1) {
		ErrorHandling("read() error!");
	}
	printf("Message from server: %s \n", message);
	while (1) {
		//try to send msg to server
		char sendmsg[100];
		printf("Enter a value :");
		gets(sendmsg);

		
		send(hSocket, sendmsg, sizeof(sendmsg), 0);
		
		char recvBuff[30];
		recv(hSocket, recvBuff, sizeof(recvBuff) - 1, 0);
		printf("Get a recvMsg : %s \n", recvBuff);
		if (sendmsg[0] == '0') break;
	}
	closesocket(hSocket);
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