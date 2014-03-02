#ifndef TELEGRAM_H
#define TELEGRAM_H
//------------------------------------------------------------------------
//
//  Name:   Telegram.h
//
//  Desc:   This defines a telegram. A telegram is a data structure that
//          records information required to dispatch messages. Messages 
//          are used by game agents to communicate with each other.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <iostream>
#include <math.h>

class GameComponent;


struct Telegram
{
	//the entity that sent this telegram
	GameComponent* Sender;

	//the entity that is to receive this telegram
	GameComponent* Receiver;

	//the message itself. These are all enumerated in the file
	//"MessageTypes.h"
	int          Msg;

	//any additional information that may accompany the message
	void*        ExtraInfo;


	Telegram():Sender(NULL),
		Receiver(NULL),
		Msg(-1)
	{}


	Telegram(GameComponent* sender,
		GameComponent* receiver,
		int    msg,
		void*  info = NULL): Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}


#endif