/////////////////////////////////////////////////////////////////////
//  AbstractCommunicator.cpp - Provides interface and 
//  abstract base classes for concrete classes to process messages from a queue
//  appropriately
//
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////

#include "AbstractCommunicator.h"

//initialising the static instance of dispatcher
AbstractDispatcher* AbstractDispatcher::ref;

//----< post message to the message queue>-------
void AbstractCommunicator::postMessage(Message msg)
{
	bq.enQ(msg);
}

//----< get message from the message queue>-------
Message AbstractCommunicator::getMessage()
{
	return bq.deQ();
}

//----< start the thread of processing messages>-------
void AbstractCommunicator::start()
{
	t = new thread(&AbstractCommunicator::processMessages, this);
}

//----< wait for thread to finish the processing >-------
void AbstractCommunicator::wait()
{
	t->join();
}

//----< destruct instance >-------
AbstractCommunicator::~AbstractCommunicator(){
	//delete t;
}

//----< defines the behaviour of handling incoming messages >-------
void AbstractCommunicator::processMessages(){};


//----< set the current instance of the dispatcher >-------
AbstractDispatcher::AbstractDispatcher(){ ref = this; }

//----< get the application wide instance of dispatcher>-------
 AbstractDispatcher& AbstractDispatcher::getInstance(){
	return *ref;
}

//----< stop the thread >-------
void AbstractDispatcher::stop(){
	Message msgStop;
	Header hdrStop;
	hdrStop.setAttrib("msg", "stop");
	hdrStop.setTargetCommunicator(this->name);
	msgStop.setHdr(hdrStop);
	this->postMessage(msgStop);
}

//----< register communicator for routing messages >-------
void AbstractDispatcher::registerComm(AbstractCommunicator* communicator)
{
	try{
		if (communicator->getName() == this->name)  // don't allow MessageDispatcher to register itself
			return;
		commMap[communicator->getName()] = communicator;
	}
	catch (std::exception ex){
		sout << ex.what();
	}
}

//----< default behavior of MessageDispatchers >-----------
void AbstractDispatcher::processMessages(){
	try{
		while (true)
		{
			Message msg = bq.deQ();

			string targetComm = msg.getHdr().getTargetCommunicator();
			string content = msg.getHdr().getAttrib("msg");

			if (commMap.find(targetComm) != commMap.end())
			{
				commMap[targetComm]->postMessage(msg);  // send on to TargetCommunicator
			}
			else if (targetComm == this->getName() && content == "stop")  // quit if sent to me
			{
				sout << "\n Stopping dispatcher\n";
				break;
			}
			else  // can't find TargetCommunicator
			{
				if (commMap.find("SenderCommunicator") != commMap.end())
					commMap["SenderCommunicator"]->postMessage(msg);
			}
		}
	}
	catch (std::exception ex){
		sout << ex.what();
	}
}
#ifdef TEST_ABSTRACT_COMMUNICATOR
#include "../Sender/Sender.h"
int main()
{
	int ret = 0;
	try
	{
		Receiver rcvr;
		rcvr.connect(8181);
		EchoCommunicator echo; // Concrete implementation of abstract communicator

		echo.setName("EchoCommunicator");
		rcvr.registerComm(&echo);
		echo.start();

		FileProcessingCommunicator fileComm;//concrete implementation 
		fileComm.setName("FileHandlerCommunicator");
		rcvr.registerComm(&fileComm);
		fileComm.start();
		string targetAddr = "127.0.0.1", targetPort = "8080";
		Sender sender;
		sender.setName("SenderCommunicator");
		////sender.connect(targetAddr, targetPort);
		rcvr.registerComm(&sender);
		sender.start();

		echo.wait();
		fileComm.wait();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch (...)
	{
		sout << "\n  something bad happened";
		ret = 1;
	}
	sout << "\n\n";

	return ret;
}
#endif
