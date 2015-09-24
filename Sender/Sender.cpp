/////////////////////////////////////////////////////////////////////
//  Sender.cpp - Implements sender functionalities of a peer to peer
// channel
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
////////////////////////////////////////////////////////////////////
#include "../sockets/Sockets.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Threads/Locks.h"
#include <string>
#include <iostream>
#include <sstream>
#include "Message.h"
#include "Header.h"
#include "FileSystem.h"
#include "Sender.h"

//Constructs instance
Sender::Sender() :targetAddr(""), targetPort(""){
	name = "SenderCommunicator";
}

//Connect to specified ip
bool Sender::connect(std::string ip, string port){
	try{
		//s_.disconnect();//disconnecting previous connection if any
		Socket* s_1 = new Socket();
		s_ = *s_1;
		if (!s_.connect(ip, atoi(port.c_str())))
		{
			string sMsg = "\n\n Sender: Couldn't Connect to " + ip + ":" + port + "\n\n";
			sout << sMsg;
			return false;
		}
		else
		{
			if (ip != targetAddr || port != targetPort){
				string sMsg1 = "\n\n Sender: Connected to " + ip + ":" + port + "\n\n";
				sout << sMsg1;
			}
			targetAddr = ip; targetPort = port;
			return true;
		}
	}
	catch (std::exception e){
		sout << e.what();
		return false;
	}
	return true;
}

//stop the processing
void Sender::stop()
{
	Message msgStop;
	Header hdrStop;
	hdrStop.setAttrib("msg", "stop");
	hdrStop.setTargetCommunicator(this->name);
	msgStop.setHdr(hdrStop);
	this->postMessage(msgStop);
}

//Process the message queue
void Sender::processMessages(){
	try{
		run();
	}
	catch (exception e){
		sout << e.what();
	}
}

//Wait for completion of processing
void Sender::wait(){
	t->join();
}

//destructor
Sender::~Sender(){}

//Run the thread
void Sender::run(){
		std::string msg_content = "";
		try{
			do{
				Message msg = bq.deQ();
				Header hdr = msg.getHdr();
				msg_content = hdr.getAttrib("msg");
				//sout << "\n sending: "<<msg.getMsg()<<"\n";
				string tmp_addr = hdr.getAddress(), tmp_port = hdr.getPort();
				if (hdr.getTargetCommunicator() == this->name && hdr.getAttrib("msg") == "stop")
				{
					sout << "\nquitting sender";
					break;
				}
				if (!reconnectIfDiff(tmp_addr, tmp_port)){
					continue;
				}
				if (!sendMsg(msg)){
					continue;
				}
				::Sleep(100);
			} while (true);
		}
		catch (exception e){
			e.what();
		}
	}

//send message
bool Sender::sendMsg(Message& msg){
	Header hdr = msg.getHdr();
	string msg_content = hdr.getAttrib("msg");
	if (hdr.getCmd() == "send_file"){
		if (!sendFile(msg)){
			sout << "\n  bad status in sending thread";
			return false;
		}
	}
	else{
		if (msg_content == "Response for quit_sender"){
			targetAddr = "";
			targetPort = "";
		}
		string sMsg = "\n Started sending msg " + msg_content+" to "+hdr.getAddress()+":"+hdr.getPort()+"\n";
		sout << sMsg;
		if (!s_.send(msg.getMsg())){
			sout << "\n  bad status in sending thread";
			return false;
		}
	}
	return true;
}

//If a different url, re initialise socket
bool Sender::reconnectIfDiff(string tmp_addr, string tmp_port){
	if (tmp_addr == targetAddr && tmp_port == targetPort){
		return true;
	}

	if (!this->connect(tmp_addr, tmp_port))
			return false;
	targetAddr = tmp_addr; targetPort = tmp_port;
	return true;
}

//Read and send blocks of file
bool Sender::sendFile(Message& msg){
		Header hdr = msg.getHdr(); string path = hdr.getAttrib("filename");
		string sMsg = "\n Started sending file " + path + " to " + hdr.getAddress() + ":" + hdr.getPort() + "\n";
		sout << sMsg;
		std::ifstream ifstream(path.c_str(), std::ios::in | std::ios::binary);
		if (ifstream.is_open())	{
			ifstream.seekg(0, ios::beg); 
			bool isStartFlagSent = false;
			while (!ifstream.eof()){
				byte block[1024];
				ifstream.read((char*)block, 1024);//Read blocks of 1024 from file to be sent to the receiver
				if (!isStartFlagSent){
					hdr.setCmd("send_file_begin");//If start of file, to open a stream at the receiver end
					isStartFlagSent = true;
				}
				else{
					hdr.setCmd("send_file");
				}
				int bytesRead = static_cast<int> (ifstream.gcount());
				hdr.setContentLength(bytesRead); msg.setHdr(hdr);
				if (!s_.send(msg.getMsg())){	//send header with content length for the body to follow
					sout << "\n  bad status in sending thread";
					break;
				}
				if (!sendBody(bytesRead, block)){ //send blocks of body
					sout << "\n  bad status in sending thread";
					break;
				}
			}
			ifstream.close();
			hdr.setCmd("end_of_file"); hdr.setContentLength(0);
			msg.setHdr(hdr);
			if (!s_.send(msg.getMsg())){
				return false;
			}
		}
		string sMsg1 = "\n Ended sending of file " + path + "\n\n";
		sout << sMsg1;

		return true;
	}

//Sends blocks of file as body
bool Sender::sendBody(int bytesRead, byte* block){
	if (bytesRead < 1024){
		byte* blockResized = new byte[bytesRead];//Sending the last block of file less than  1024
		for (int i = 0; i < bytesRead; i++){
			blockResized[i] = (block[i]);
		}
		if (!s_.sendAll((const char*)(blockResized), bytesRead)){
			return false;
		}
		delete[] blockResized;
	}
	else{
		if (!s_.sendAll((const char*)block, bytesRead)){
			return false;
		}
	}
	return true;
}

#ifdef TEST_SENDER
#include "../Reciever/Receiver.h"
int main()
{
  int ret = 0;
  try
  {
	string srcAddr = "127.0.0.1", srcPort ="8484" , targetAddr = "127.0.0.1", targetPort ="8181" ,  targetCommunicator = "file_processor";
	Message msg, msgEcho; Header hdr, hdrEcho;
	hdr.setCmd("send_file");
	hdr.setAddr(targetAddr, targetPort);//IP address
	hdr.setAttrib("filename","D:/proj3/SocketDemo2/Test/sender/foobar.txt");//name value pairs
	hdr.setAttrib("msg", "File transfer");
	hdr.setSrcAddr(srcAddr, srcPort);
	hdr.setContentLength(1024);
	hdr.setTargetCommunicator("FileHandlerCommunicator");
	msg.setHdr(hdr);
	hdrEcho.setCmd("echo_msg");
	hdrEcho.setAttrib("msg", "Hi there!");
	hdrEcho.setAddr(targetAddr, targetPort);//IP address
	hdrEcho.setSrcAddr(srcAddr, srcPort);
	hdrEcho.setTargetCommunicator("EchoCommunicator");
	msgEcho.setHdr(hdrEcho);
	Sender sndr1;
	int clientPort = 8484;
	Receiver rcvr;
	rcvr.connect(clientPort);
	EchoCommunicator echo;
	echo.setName(CLientEchoCommunicator);
	rcvr.registerComm(&echo);
	echo.start();
	sndr1.start();
	sndr1.postMessage(msgEcho);
	sndr1.e(msg);
	echo.wait();
	rcvr.wait();
	sndr1.wait();
	sout << "That's all folks!";
  }
  catch(std::exception& ex)
  {
    sout << "\n\n  " << ex.what();
    ret = 1;
  }
  catch(...)
  {
    sout << "\n\n  something bad happend to a sender";
    ret = 1;
  }
  std::cout << "\n\n  That's all Folks!\n\n";
  return ret;
}
#endif