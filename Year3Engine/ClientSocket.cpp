#include "ClientSocket.h"


ClientSocket::ClientSocket(TCPSocket* socket)
	:TCPSocket(socket)
{
}


ClientSocket::~ClientSocket()
{

}

//on ready handler
void ClientSocket::OnReady()
{
	//read from socket
	//string s = Receive();
	//notify application
	//CApplication::GetApplication()->SendMessage(MSGID_SocketData,(MSGPARM)this,(MSGPARM)s.c_str());
}