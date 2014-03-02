#pragma once

#include "TCPSocket.h"

class HostSocket : public TCPSocket
{
public:
	//constructor
	HostSocket(IP_Address* ip);
	//destructor
	virtual ~HostSocket(void);
	//when ready with data
	void OnReady();
	//creation
	static HostSocket* Create(Uint16 port);
};

