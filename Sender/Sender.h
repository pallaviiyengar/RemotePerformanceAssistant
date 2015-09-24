/////////////////////////////////////////////////////////////////
// Sender.h - sender class which is a communicator, with its own queue and socket
// each incoming message checked and sent appropriately.
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:	   Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message passing, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar				 
//  Source:		   Jim Fawcett
/////////////////////////////////////////////////////////////////
/*
* Module operation:
* This class send messages to a specified end point
*
* Follows HTTP like protocols, with lines in the header and bytes in the body, if there is one,
* specified by a header line something like:
*    content_length : 1024
* where 1024 is a stand-in for whatever you want your block
* size to be.
*
* For a file, 1024 bytes are read(except for the last block), each block is sent as body, preceded by header indicating number of bytes to follow
*
* For a text message, the message is sent in the header.
*
* Public interface:
* Sender sndr1;
* sndr1.start();
* Message msgEcho; Header Echo;
* string srcAddr = "127.0.0.1", srcPort ="8484" , targetAddr = "127.0.0.1", targetPort ="8181";
* hdrEcho.setCmd("echo_msg");
* hdrEcho.setAttrib("msg", "Hi there!");
* hdrEcho.setAddr(targetAddr, targetPort);//IP address
* hdrEcho.setSrcAddr(srcAddr, srcPort);
* hdrEcho.setTargetCommunicator(COMM_ECHO);
* msgEcho.setHdr(hdrEcho);
* sndr1.postMessage(msgEcho);
* sndr1.stop();
* sndr1.wait();
*
*Build Process:
*- devenv MessagePassing.sln
*
* Required files:
* - Sockets.h, Sockets.cpp, BlockingQueue.h, BlockingQueue.cpp
*   AbstractCommunicator.h, AbstractCommunicator.cpp
*   Message.h, Message.cpp. Header.h
*   BlockingQueue.h, BlockingQueue.cpp
*   Locks.h, Locks.cpp
*   FileSystem.h, FileSystem.cpp
*
* Maintanence History:
* ver 1.0 - 10 Apr 2014
* - first release
*/
#ifndef SENDER_H
#define SENDER_H
#include "../sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Threads/Locks.h"
#include <string>
#include <iostream>
#include <sstream>
#include "Message.h"
#include "Header.h"
#include "FileSystem.h"
#include "../Communicator/AbstractCommunicator.h"

using namespace FileSystem;
using namespace std;

class Sender : public AbstractCommunicator
{
public:
	Sender();

	void wait();
	void stop();

	virtual ~Sender();

private:
	void processMessages();
	bool connect(std::string ip, string port);
	void run();

	bool sendMsg(Message& msg);
	bool sendFile(Message& msg);
	bool sendBody(int bytesRead, byte* block);
	bool reconnectIfDiff(string tmpAddr, string tmpPort);
	
	string targetAddr;
	string targetPort;
	string srcAddr;
	string srcPort;
};

#endif SENDER_H