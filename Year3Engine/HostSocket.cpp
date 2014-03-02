#include "HostSocket.h"


HostSocket::HostSocket(IP_Address* ip)
	:TCPSocket(ip)
{
}


HostSocket::~HostSocket(void)
{
}

//when ready with data
void HostSocket::OnReady()
{
}

//creation
HostSocket* HostSocket::Create(Uint16 port)
{
	//create the ip
	IP_Address ip(NULL, port);
	//create socket
	return new HostSocket(&ip);
}
