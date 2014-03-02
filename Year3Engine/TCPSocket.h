#pragma once

#include <SDL\SDL_net.h>
#include "IP_Address.h"

#include <list>
#include <string>

using namespace std;

enum { NETTCP_PACKETSIZE_DEFAULT = 256 };

class TCPSocket
{
public:
	//uses SDLNet_TCP_Open
	TCPSocket(IP_Address* ipAddress);

	//create from an already existing TCPSocket
	TCPSocket(TCPsocket socket);

	//uses SDLNet_TCP_Accept
	TCPSocket(TCPSocket* socket);

	//close socket
	virtual ~TCPSocket();

	//retrieve master socket list
	static list<TCPSocket*>& SocketList();

	//accessors
	TCPsocket& Socket();

	//conversion operator
	operator TCPsocket();

	//get the IP address
	IP_Address GetIP();

	bool GetStatus();

	//send a string
	bool Send(string data);

	//receive a string
	string Receive();

	//is there data to be read?
	bool Ready();

	//check this socket
	void Check();

	//when a check proves that a socket is ready, call this function
	virtual void OnReady();

	//check all sockets
	static void CheckAll();

private :
	//internal representation
	TCPsocket socket;
	//static list of all sockets
	static list<TCPSocket*> sockets;
};

