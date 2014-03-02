#pragma once

//include sdl_net
#include <SDL\SDL_net.h>

class IP_Address
{
public:
	// Constructor
	IP_Address(char* host, Uint16 port);
	//construct from an IPaddress
	IP_Address(IPaddress* ipAddress);
	//destructor
	virtual ~IP_Address(void);
	//get host
	Uint32 GetHost();
	//get port
	Uint16 GetPort();
	//resolve the IP address
	const char* Resolve();
	//conversion operator
	operator IPaddress*();
private:
	IPaddress ip; // SDL_Net representation
};

