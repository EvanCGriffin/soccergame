#include "TCPSocket.h"

list<TCPSocket*> TCPSocket::sockets;

//uses SDLNet_TCP_Open
TCPSocket::TCPSocket(IP_Address* ipAddress) 
{
	SocketList().push_back(this);

	Socket() = SDLNet_TCP_Open(*ipAddress);
}

//create from an already existing TCPSocket
TCPSocket::TCPSocket (TCPsocket socket) 
{
	SocketList().push_back(this);

	Socket() = socket;
}

//uses SDLNet_TCP_Accept
TCPSocket::TCPSocket(TCPSocket* socket) 
{
	SocketList().push_back(this) ;

	Socket() = SDLNet_TCP_Accept(*socket);
}

//close socket
TCPSocket::~TCPSocket()
{
	SocketList().remove(this);

	SDLNet_TCP_Close(*this);
}

// retrieve master socket list
list<TCPSocket*>& TCPSocket::SocketList () 
{
	return sockets;
}

//accessors
TCPsocket& TCPSocket::Socket() 
{
	return socket;
}

//conversion operator
TCPSocket::operator TCPsocket() 
{
	return socket;
}

//get the IP address
IP_Address TCPSocket::GetIP ( ) 
{
	IPaddress* ipaddress = SDLNet_TCP_GetPeerAddress(*this);
	if (ipaddress)
	{
		return (IP_Address(ipaddress));
	}
	else
	{
		IPaddress ip;
		ip.host=0;
		ip.port=0;
		return (IP_Address(&ip));
	}
}

//send a string
bool TCPSocket::Send(string data) 
{
	if (data.size() == 0) 
		return true;

	char* buffer = _strdup(data.c_str());

	int nBytesSent = SDLNet_TCP_Send(*this, buffer, data.size());

	free (buffer);

	return (nBytesSent == (int)data.size());
}

//receive a string
string TCPSocket::Receive() 
{
	if (!Ready()) 
		return ("");

	char* buffer = new char[NETTCP_PACKETSIZE_DEFAULT + 1];

	string sData = "";

	int nBytesReceived; 

	while (Ready()) 
	{
		memset (buffer, 0, NETTCP_PACKETSIZE_DEFAULT + 1);
	
		nBytesReceived = SDLNet_TCP_Recv(*this, buffer, NETTCP_PACKETSIZE_DEFAULT);

		if (nBytesReceived <= 0)
		{
			delete[] buffer;
			return sData;
		}

		sData += buffer;
	}

	delete[] buffer;

	return sData;
}

bool TCPSocket::Ready() 
{
	return SDLNet_SocketReady(socket);
}

//check this socket
void TCPSocket::Check() 
{
	if (!Socket()) 
		return;

	SDLNet_SocketSet set = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(set, Socket());
	SDLNet_CheckSockets(set, 0);
	SDLNet_FreeSocketSet(set);
	
	//if the socket is ready, call the onready function
	if (Ready())
		OnReady();
}

//when a check proves that a socket is ready, call this function
void TCPSocket::OnReady() 
{
	//by default, do nothing
}

//check all sockets
void TCPSocket::CheckAll ( ) 
{
	list<TCPSocket*>::iterator iter;
	TCPSocket* temp;
	for (iter = SocketList().begin(); iter != SocketList().end(); iter++)
	{
		temp = *iter;
		temp->Check();
	}
}

bool TCPSocket::GetStatus() 
{
	return (socket != NULL);
}