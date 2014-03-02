#pragma once

#include "TCPSocket.h"

class ClientSocket : public TCPSocket
{
public:
	//constructor
	ClientSocket(TCPSocket* socket);
	//destructor
	virtual ~ClientSocket();
	//on ready handler
	void OnReady();
};

