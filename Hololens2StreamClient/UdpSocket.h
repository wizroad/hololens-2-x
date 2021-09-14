#pragma once

#include <WinSock2.h>
#include <iostream>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

#define serverIP "192.168.0.3"
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
		addr.sin_addr.s_addr = inet_addr(serverIP);
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
	int SendBytes(byte* bytearray, int size)
	{
		return sendto(server, (char*)bytearray, size, 0, (sockaddr*)&addr, sizeof(addr));
	}
};
