
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
This module provides methods to construct and interpret HTTP like messages.

Public Interface :
== == == == == == == == =
Message msg;
hdrEcho.setCmd("echo_msg");
hdrEcho.setAttrib("msg", "Hi there!");
hdrEcho.setAddr(targetAddr, targetPort);//IP address
hdrEcho.setSrcAddr(srcAddr, srcPort);
hdrEcho.setTargetCommunicator(COMM_ECHO);
msg.setHdr(hdrEcho);

string strMsg;
Message  recvdMsg;
recvdMsg.parseMsg(strMsg);
recvdMsg.getHdr();

recvdMsg = msg;

Build Process :
== == == == == == ==
Required files
- Does not depend on any other files, other than built-in c++ functions.
Build commands(either one)
- devenv MessagePassing.sln

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 11 Apr 2014
- first release
*/

#include "../Sender/Message.h"

//Parse a string into message parts
void Message::parseMsg(string msg){
	try{
		string cmd = find("command=", msg);
		hdr.setCmd(cmd);
		string addr = find("address=", msg); string port = find("port=", msg);
		hdr.setAddr(addr, port);
		string targ_comm = find("target_communicator=", msg);
		hdr.setTargetCommunicator(targ_comm);
		string src_addr = find("src_address=", msg); string src_port = find("src_port=", msg);
		hdr.setSrcAddr(src_addr, src_port);
		string src_comm = find("src_communicator=", msg);
		hdr.setSrcCommunicator(src_comm);
		string length = find("content_length=", msg);
		hdr.setContentLength(atoi(length.c_str()));
		string filename = find("filename=", msg);
		hdr.setAttrib("filename", filename);
		string content = find("msg=", msg);
		hdr.setAttrib("msg", content);
		string dir = find("directory=", msg);
		hdr.setAttrib("directory", dir);

		string srchString = find("search_string=", msg);
		hdr.setAttrib("search_string", srchString);
		string numThreads = find("no_of_threads=", msg);
		hdr.setAttrib("no_of_threads", numThreads);
		string processing_time = find("time=", msg);
		hdr.setAttrib("time", processing_time);
		string start_time = find("start_time=", msg);
		hdr.setAttrib("start_time", start_time);
		string stop_time = find("stop_time=", msg);
		hdr.setAttrib("stop_time", stop_time);
		string num_files = find("num_of_files=", msg);
		hdr.setAttrib("num_of_files", num_files);
		int nFiles = atoi(num_files.c_str());
		for (int i = 0; i < nFiles; i++){
			string name = "file" + to_string(i) ;
			string foundFile = find(name+"=", msg);
			hdr.setAttrib(name, foundFile);
		}
	}
	catch (exception e){
		sout << e.what();
	}
}

//Builds and returns message associated with this instance
string Message::getMsg(){
	constructMsg();
	return msg;
}

//Assignmet operator overload
Message& Message::operator=(const Message& msg2){
	this->body = msg2.body;
	this->hdr = msg2.hdr;
	this->msg = msg2.msg;
	return *this;
}

//Get header of a message
Header Message::getHdr(){
	return hdr;
}

//Sets header of a message
void Message::setHdr(Header _hdr){
	this->hdr = _hdr;
}

//prints out the message
void Message::showMessage(){
	string content = hdr.getAttrib("msg");
	sout << "\n" << content;
}

//Constructs the message from different header parts. Header ends with a new line.
void Message::constructMsg(){
	msg = "";
	try{
		msg = "command=" + hdr.getCmd() + "--"
			+ "address=" + hdr.getAddress() + "--"
			+ "port=" + hdr.getPort() + "--"
			+ "target_communicator=" + hdr.getTargetCommunicator() + "--"
			+ "src_address=" + hdr.getSrcAddr() + "--"
			+ "src_port=" + hdr.getSrcPort() + "--"
			+ "src_communicator=" + hdr.getSrcCommunicator() + "--"
			+ "content_length=" + to_string(hdr.getContentLength()) + "--";

		unordered_map<std::string, std::string>::iterator iter;
		unordered_map<string, string> mapAttrib = hdr.getAttribs();
		for (iter = mapAttrib.begin(); iter != mapAttrib.end(); ++iter) {
			msg += iter->first + "=" + iter->second + "--";
		}
	}
	catch (exception e){
		sout << e.what();
	}
	msg += "\r\n";
}

//Find in the header a specified part like address, communicator etc.
string Message::find(string _hdr, string msg){
	string cmd = "";
	try{
		std::size_t cmd_hdr_start = msg.find(_hdr);
		if (cmd_hdr_start != std::string::npos){
			std::size_t cmd_hdr_end = cmd_hdr_start + _hdr.length();
			std::size_t cmd_end = msg.find("--", cmd_hdr_end);
			if (cmd_end != std::string::npos){
				cmd = msg.substr(cmd_hdr_end, (cmd_end - cmd_hdr_end));
				return cmd;
			}
		}
	}
	catch (exception e){
		sout << "\n  " << e.what();
	}
	return cmd;
}


//Constructs message instance
Message::Message(){}

//Destructs an instance

Message::~Message(){}

//Test stub
#ifdef TEST_MESSAGE
int main(){
	Message msg;
	hdrEcho.setCmd("echo_msg");
	hdrEcho.setAttrib("msg", "Hi there!");
	hdrEcho.setAddr(targetAddr, targetPort);//IP address
	hdrEcho.setSrcAddr(srcAddr, srcPort);
	hdrEcho.setTargetCommunicator(COMM_ECHO);
	msg.setHdr(hdrEcho);
	
	string strMsg = msg.getMsg();
	cout<<"Message is : " << strMsg;
	Message  recvdMsg;
	recvdMsg.parseMsg(strMsg);
	
	Header hdr = recvdMsg.getHdr();
	cout<< "target addr : " << hdr.getAddress();
	cout<<"target port : "<< hdr.getPort();
	cout<<"src addr:" << hdr.getSrcAddr();
	cout<<"src port:"<< hdr.getSrcPort();
	cout<<"target comm:"<<hdr.getTargetCommunicator();
	cout<<"cmd:"<< hdr.getTargetCommunicator();
	cout<<"msg:"<<hdr.getAttrib("msg");
}

#endif