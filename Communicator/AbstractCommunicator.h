/////////////////////////////////////////////////////////////////////
//  AbstractCommunicator.h -  This class describes interface and 
//  abstract base class for concrete classes that process messages 
// and handle message dispatching on child threads
//																   // 
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
* ICommunicator
*	This  provides a contract for sending messages and naming.
*  
AbstractCommunicator
*    Serves as a base class for named objects that process messages on child threads.
*
*  AbstractMessageDispatcher
*    Is a base class for mediators that handle message dispatching.  The dispatcher
*    is the first place a message goes when delivered by a communication channel.
*    Its job is to route messages based on the names of the intended targets.

Build Process:
==============
Build commands
- devenv MessagePassing.sln

Required files
==============
Message.h, Sockets.h, BlockingQueue.h

Maintenance History:
====================
ver 1.0 : 10 Apr 2014
- first release

*/
//
#ifndef ABSTRACT_COMMUNICATOR_H
#define ABSTRACT_COMMUNICATOR_H
#include "../Sockets/Sockets.h"
#include "../Sender/Message.h"
#include "../BlockingQueue/BlockingQueue.h"
#include <thread>
#include <string>

using namespace std;

class ICommunicator{
public:
	//----< post message to a message queue>-------
	virtual void postMessage(Message)=0;

	//----< return name of the communicator>-------
	inline string getName(){
		return name;
	}

	//----< set name of the communicator>-------
	inline void setName(string _name){
		name = _name;
	}

	//----< destruct the instance >-------
	virtual ~ICommunicator(){}

protected:
	string name;

};

///////////////////////////////////////////////////////////////////////
// AbstractCommunicator 
// - defines parts that accept messages
// - each Communicator has a thread for processing messages
// - behavior is defined by override of processMessages()
class AbstractCommunicator : public  ICommunicator{
protected :
	Socket s_;
	BlockingQueue<Message> bq;
	std::thread* t;

public:
	//----< post message to the message queue>-------
	virtual void postMessage(Message msg);

	//----< get message from the message queue>-------
	Message getMessage();

	//----< start the thread of processing messages>-------
	virtual void start();

	//----< wait for thread to finish the processing >-------
	virtual void wait();

	//----< destruct instance >-------
	virtual ~AbstractCommunicator();

	//----< defines the behaviour of handling incoming messages >-------
	virtual void processMessages();
};

///////////////////////////////////////////////////////////////////////
// AbstractMessageDispatcher 
// - serves as message routher between message-passing parts
class AbstractDispatcher : public AbstractCommunicator{
private:
	unordered_map<string, AbstractCommunicator*> commMap;
	static AbstractDispatcher* ref;

protected:

public:
	//----< construct the instance >-------
	AbstractDispatcher();

	//----< get the application wide instance of dispatcher>-------
	static AbstractDispatcher& getInstance();

	//----< stop the thread >-------
	void stop();

	//----< register communicator for routing messages >-------
	virtual void registerComm(AbstractCommunicator* communicator);
	
	//----< default behavior of MessageDispatchers >-----------
	virtual void processMessages();

};

#endif