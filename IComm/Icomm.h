/////////////////////////////////////////////////////////////////////
//  IComm.h -  Interface for component using C++ 11 thread
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
* This module provides classes IComm which acts an interface component 
* for using C++11 threads, for application such as C++/CLI applications
*
* Public Interface:
* =================
string srcAddr = "127.0.0.1", srcPort = "8080", targetAddr = "127.0.0.1", targetPort = "8181", dir = ".\\UploadFiles", srchStr = "hi", numThreads = 3, numReqs = 3;
IComm* pIComm = IComm::Create();
pIComm->setSrcAddr(srcAddr, srcPort);
pIComm->start();
pIComm->postEchoMessage(targetAddr, targetPort, "hello");
string echo = pIComm->getEchoMessage();
pIComm->upload(targetAddr, targetPort, dir);
pIComm->download(targetAddr, targetPort, "127.0.0.1_8484_zzzzimage.jpg");
pIComm->searchString(targetAddr, targetPort, dir, srchStr, numThreads);
pIComm->stop(targetAddr, targetPort);
pIComm->wait();
* Required Files:
* ===============
* string.h
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
#ifndef ICOMM_H
#define ICOMM_H

#include <string>
using namespace std;
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

struct IComm{
	//virtual DLL_DECL ~IComm();
	static DLL_DECL IComm* Create();
	virtual DLL_DECL void setSrcAddr(string srcAddr, string srcPort)=0;
	virtual DLL_DECL void postEchoMessage(string targetAddr, string targetPort, string message)=0;
	virtual DLL_DECL int upload(string targetAddr, string targetPort, string dir)=0;
	virtual DLL_DECL void download(string targetAddr, string targetPort, string file)=0;
	virtual DLL_DECL void searchString(string targetAddr, string targetPort, string targetDir, string str, string noOfThreads, string nReqs)=0;
	virtual DLL_DECL string getEchoMessage()=0;
	virtual DLL_DECL void start() = 0;
	virtual DLL_DECL void wait() = 0;
	virtual DLL_DECL void stop(string targetAddr, string targetPort) = 0;
};

#endif