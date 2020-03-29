#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MSG_SIZE 512
#define UPDATE_INTERVAL 100

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA WsaData;
	
	//intalize out winsock
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0 )
	{
		std::cout << "WSA failed! \n";
		WSACleanup();
		return 0;
	}


	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //it is TCP
	if (Socket == INVALID_SOCKET)
	{
		std::cout << "Socket creation error\n";
		WSACleanup();
		return 0;
	}

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(8888);
	SockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &SockAddr.sin_addr.s_addr);

	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		std::cout << "Failed to connect to server\n";
		return 0;
	}

	//send and recieve infromation so we nee 2 buffers

	//can also use memsize of strcpy
	char recBuf[MSG_SIZE] = { "" };
	char sendBuf[MSG_SIZE] = { "" };

	//change to non blocking mode
	u_long mode = 1;
	ioctlsocket(Socket, FIONBIO, &mode);

	int c = 0; 
	while (true)
	{
		strcpy(sendBuf, "");
		//increment c and store it in the buffer and do that over and over as if they were position updates
		std::sprintf(sendBuf, "%d", c++); 

		//we have to send in interval
		//can put this in threads if you wanted to make more interesitn 
		//sleep tehn send message
		//one thread for sending
		//one thread for recievin
		//create one single thread for receiving
		Sleep(UPDATE_INTERVAL);

		if (send(Socket, sendBuf, sizeof(sendBuf), 0) == 0)
		{
			std::cout << "Failed to send!\n";
			closesocket(Socket);
			WSACleanup();
			break;
		}

		if (recv(Socket, recBuf, sizeof(recBuf), 0) > 0)
		{
			std::cout << recBuf << std::endl;
		}
	}

	shutdown(Socket, SD_BOTH);
	closesocket(Socket);
	WSACleanup();
	return 1; 
	 
}
