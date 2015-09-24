/////////////////////////////////////////////////////////////////////
//  Comm.h -  Implements interface for communication channel for components using C++ 11 thread
//																   // 
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module implements communication channel interface  providing facilities of communicaition
* like  starting communication, sending and receivingfiles and messages and stopping server.
*
* Public Interface:
* =================
string srcAddr = "127.0.0.1", srcPort = "8080", targetAddr = "127.0.0.1", targetPort = "8181", dir = ".\\UploadFiles", srchStr = "hi", numThreads = 3, numReqs = 3;
Comm* comm = new Comm();//This creates instance of Comm class
comm->setSrcAddr(srcAddr, srcPort);
comm->start();
comm->postEchoMessage(targetAddr, targetPort, "hello");
string echo = comm->getEchoMessage();
comm->upload(targetAddr, targetPort, dir);
comm->download(targetAddr, targetPort, "127.0.0.1_8484_zzzzimage.jpg");
comm->searchString(targetAddr, targetPort, dir, srchStr, numThreads);
comm->stop(targetAddr, targetPort);
comm->wait();
* Required Files:
* ===============
* Icomm.h, Receiver.h, Sender.h, Header.h, Message.h, FileManager.h, ChronoTimer.h
*
* Build Command:
* ==============
- devenv MessagePassing.sln
*
* Maintenance History:
* ====================
* ver 1.0 : 26 Apr 2014
* - first release
*/
#ifndef COMM_I_H
#define COMM_I_H

#pragma once
#include "Icomm.h"
#include "../Reciever/Receiver.h"
#include "../Sender/Sender.h"
#include "../Sender/Header.h"
#include "../Sender/Message.h"
#include "../ClientExecutive/FileManager.h"
#include "../ChronoTimer/ChronoTimer/ChronoTimer.h"
class Comm : public IComm
{
public:
	Comm();
	//~Comm();
	void postEchoMessage(string targetAddr, string targetPort, string message);
	string getEchoMessage();

	int upload(string targetAddr, string targetPort, string dir);
	void download(string targetAddr, string targetPort, string file);
	void searchString(string targetAddr, string targetPort, string targetDir, string str, string noOfThreads, string nReqs);

	void setSrcAddr(string srcAddr, string srcPort);
	void start();
	void wait();
	void stop(string targetAddr, string targetPort);

private:
	string srcAddr, srcPort;
	Receiver rcvr;
	ClientEchoCommunicator echo; 
	Sender sndr; 
};
#endif