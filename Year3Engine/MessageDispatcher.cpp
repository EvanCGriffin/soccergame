#include "MessageDispatcher.h"
#include "GameComponent.h"

using std::set;

//uncomment below to send message info to the debug window
//#define SHOW_MESSAGING_INFO

//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
MessageDispatcher* MessageDispatcher::Instance()
{
  static MessageDispatcher instance; 
  
  return &instance;
}

//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::Discharge(GameComponent* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    //telegram could not be handled
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "Message not handled" << "";
    #endif
  }
}

//---------------------------- DispatchMsg ---------------------------
//
//  given a message, a receiver, a sender and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageDispatcher::DispatchMsg(GameComponent* sender, GameComponent* receiver, 
	int msg, void* AdditionalInfo = NULL)
{
  //make sure the receiver is valid
  if (receiver == NULL)
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nWarning! No Receiver with ID of " << receiver << " found" << "";
    #endif

    return;
  }
  
  //create the telegram
  Telegram telegram(sender, receiver, msg, AdditionalInfo);
  
  //send the telegram to the recipient
  Discharge(receiver, telegram);
}


