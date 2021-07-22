#pragma once

#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

#define PORT 9005
#define PACKET_SIZE 1024

class UdpSocket
{
public:
	SOCKET server;
	sockaddr_in addr = {};
	UdpSocket()
	{
		WSADATA data;
		::WSAStartup(MAKEWORD(2, 2), &data);

		server = socket(AF_INET, SOCK_DGRAM, 0);
		 
		addr.sin_family = AF_INET;
		//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_addr.s_addr = inet_addr("192.168.0.3");
		//addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(PORT);


	}

	~UdpSocket()
	{
		closesocket(server);
		WSACleanup();
	}

	int SendMsg(const char* Msg)
	{
		return sendto(server, Msg, strlen(Msg), 0, (sockaddr*)&addr, sizeof(addr));
	}

	int SendMsg(wchar_t* Msg)
	{
		return sendto(server, (char*)Msg, wcslen(Msg) * sizeof(wchar_t), 0, (sockaddr*)&addr, sizeof(addr));
	}

	int SendFrame(uint8_t* bitmap, const size_t fileSize)
	{
		return sendto(server, (char*) bitmap, fileSize, 0, (sockaddr*)&addr, sizeof(addr));
	}
};