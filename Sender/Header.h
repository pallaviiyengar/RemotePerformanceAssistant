/////////////////////////////////////////////////////////////////////
//  Header.h - This class provides a format for HTTP like headers  //
//  with content length attribute if a body is present.			   //
//  Each header ends with a new line.                              //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations :
== == == == == == == == ==
This module defines a class that  provides a format for HTTP like headers
 with content length attribute if a body is present. 
 The first attribute in the header is the command, which in the case of this application is
 send_file related to sending file to server and echo_msg to send and echo plain text message at server.
 Each header ends with a new line. It also allows to pass a set of name value
 pairs. 
 It provides operations to get and set the target and source address, communicators, name value pairs such as filename,
 content length and so on

Public Interface :
== == == == == == == == =
Header hdr;
hdr.setCmd("send_file"); //Command send file
hdr.setAddr(targetAddr, targetPort);//target address
hdr.setSrcAddr(srcAddr, srcPort);
hdr.setAttrib("filename", "D:/proj3/SocketDemo2/Test/sender/foobar.txt");//name value pairs
hdr.setAttrib("msg", "File transfer");//name value pairs
hdr.setSrcCommunicator("SenderCommunicator");
hdr.setTargetCommunicator("FileHandlerCommunicator");
hdr.setContentLength(1024);

Header hdr1;
Message msgMsg;
hdr1.setCmd("echo_msg"); //Command echo msg
hdr1.setAttrib("msg", str_msg);
hdr1.setAddr(targetAddr, targetPort);
hdr1.setSrcAddr(srcAddr, srcPort);
hdr1.setTargetCommunicator("EchoCommunicator");
msgMsg.setHdr(hdr1);
sndr1.postMessage(msgMsg);

string cmd = hdr.getAttrib("send_file");
string addr = hdr.getAddress();
string port = hdr.getPort();
string srcAddr = hdr.getSrcAddr();
string srcPort = hdr.getSrcPort();
string content = hdr.getAttrib("filename");
string msg = hdr.getAttrib("msg");
string src_communicator = hdr.getSrcCommunicator();
string target_communicator = hdr.getTargetCommunicator();
int len = hdr.getContentLength();

Build Process :
== == == == == == ==
Build commands(either one)
- devenv MessagePassing.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 11 Apr 2014
- first release
*/

#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <vector>
#include <unordered_map>
#include "../Threads/Locks.h"
using namespace std;

//Header class provides functions to construct a HTTP like header
class Header{
private:
	string cmd;
	string addr;
	string port;
	int content_length;
	unordered_map<string, string> mapAttrib;
	string srcAddress;
	string srcPort;
	string srcCommunicator;
	string targetCommunicator;

public:
	typedef unordered_map<string, string> nameValuePair;
	
	//Contructs an empty header
	Header() : content_length(0){}

	//Get command associated with the header
	inline string Header::getCmd(){
		return cmd;
	}

	//Get target address associated of the header
	inline string Header::getAddress(){
		return addr;
	}

	//Get target port 
	inline string Header::getPort(){
		return port;
	}

	//Get attributes associated with the header
	inline nameValuePair Header::getAttribs(){
		return mapAttrib;
	}

	//Gets the specified attribute
	inline string Header::getAttrib(string name){
		string ret = "";
		try{
			nameValuePair::const_iterator it = mapAttrib.find(name);
			if (it != mapAttrib.end()){
				ret = it->second;
			}
		}
		catch (std::exception e){
			cout << e.what();
		}
		return ret;
	}

	//Gets the source address
	inline string Header::getSrcAddr(){
		return srcAddress;
	}

	//Gets the source port
	inline string Header::getSrcPort(){
		return srcPort;
	}

	//Gets the source communicator
	inline string Header::getSrcCommunicator(){
		return srcCommunicator;
	}

	//Gets the target communicator
	inline string Header::getTargetCommunicator(){
		return targetCommunicator;
	}

	//Gets the content length of the header
	inline int Header::getContentLength(){
		return content_length;
	}

	//Sets the command in the header, which exxentially is an action to be performed
	inline void Header::setCmd(string _cmd){
		cmd = _cmd;
	}

	//Sets the target addr
	inline void Header::setAddr(string _addr, string _port){
		addr = _addr;
		port = _port;
	}

	//Sets the target port
	inline void Header::setPort(string _port){
		port = _port;
	}

	//Sets a name-value pair in the header
	inline void Header::setAttrib(string name, string value){
		mapAttrib[name] = value;
	}

	//Sets the content length of the body to follow
	inline void Header::setContentLength(int len){
		content_length = len;
	}

	// Sets the src  address
	inline void Header::setSrcAddr(string addr, string port){
		srcAddress = addr;
		srcPort = port;
	}

	// Sets the source communicator
	inline void Header::setSrcCommunicator(string comm){
		srcCommunicator = comm;
	}

	// Sets the target communicator to who this header has to be dispatched
	inline void Header::setTargetCommunicator(string comm){
		targetCommunicator = comm;
	}

};
#endif
