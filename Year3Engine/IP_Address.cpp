#include "IP_Address.h"

//constructor
IP_Address::IP_Address(char* host, Uint16 port)
{
	//resolve host and port
	SDLNet_ResolveHost(&ip, host, port);
}

//construct from an IPaddress
IP_Address::IP_Address(IPaddress* ipAddress)
{
	//copy from supplied ip address
	ip.host=ipAddress->host;
	ip.port=ipAddress->port;
}

IP_Address::~IP_Address(void)
{
	// Do Nothing
}

//get host
Uint32 IP_Address::GetHost()
{
	//return the host
	return ip.host;
}

//get port
Uint16 IP_Address::GetPort()
{
	//return the port
	return ip.port;
}

//conversion operator
IP_Address::operator IPaddress *()
{
	//return pointer to IPaddress
	return(&ip);
}

//resolve the IP address
const char* IP_Address::Resolve()
{
	//resolve the ip
	return SDLNet_ResolveIP(&ip);
}