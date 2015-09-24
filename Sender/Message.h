/////////////////////////////////////////////////////////////////////
//  Message.h - This module provides methods to construct and interpret 
// HTTP-like messages on both the server and client side.
//
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations :
== == == == == == == == ==
This module provides methods to construct and interpret HTTP like messages.

Public Interface :
== == == == == == == == =
Message msg;
hdrEcho.setCmd("echo_msg");
hdrEcho.setAttrib("msg", "Hi there!");
hdrEcho.setAddr(targetAddr, targetPort);//IP address
hdrEcho.setSrcAddr(srcAddr, srcPort);
hdrEcho.setTargetCommunicator("EchoCommunicator");
msg.setHdr(hdrEcho);

string strMsg = msg.getMsg();
Message  recvdMsg;
recvdMsg.parseMsg(strMsg);
Header hdr = recvdMsg.getHdr();

recvdMsg = msg;

Build Process :
== == == == == == ==
Required files
- Header.h, Header.cpp

Build commands
- devenv MessagePassing.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 11 Apr 2014
- first release
*/


#ifndef MESSAGE_H
#define MESSAGE_H

#include "Header.h"

class Message{
public:
	Message();
	~Message();

	void parseMsg(string msg);
	string getMsg();

	Message& operator=(const Message& msg2);

	Header getHdr();
	void setHdr(Header _hdr);

	void setBody(string _body);

	string getBody();

	void showMessage();

private:
	Header hdr;
	string body;
	string msg;
	void constructMsg();
	string find(string _hdr, string msg);

};

#endif