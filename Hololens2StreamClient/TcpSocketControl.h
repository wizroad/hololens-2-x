#pragma once


#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

#define PORT 9005
#define PACKET_SIZE 1024

class TcpSocketControl
{
public:
	SOCKET server;
	SOCKET client;
	TcpSocketControl()
	{
		WSADATA data;
		::WSAStartup(MAKEWORD(2, 2), &data);

		server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in addr = {};

		addr.sin_family = AF_INET;
		//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_addr.s_addr = inet_addr("192.168.0.4");
		//addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(PORT);

		if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			exit(-1);
		}

		if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
			exit(-1);
		}

		sockaddr_in caddr = {};
		int csize = sizeof(caddr);
		client = accept(server, (sockaddr*)&caddr, &csize);

	}

	~TcpSocketControl()
	{
		closesocket(client);
		closesocket(server);

		WSACleanup();
	}

	int SendMsg(const char* Msg)
	{
		return send(client, Msg, strlen(Msg), 0);
	}

	int SendMsg(wchar_t * Msg)
	{
		return send(client, (char *)Msg, wcslen(Msg) * sizeof(wchar_t), 0);
	}
};