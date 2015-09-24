#ifndef TASK_H
#define TASK_H
///////////////////////////////////////////////////////////////
// Task.h - Public interface of long running Task            //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////

#include "ITask.h"
#include <vcclr.h>
#include "../../IComm/IComm.h"
#include <string>
using namespace std;

class Task
{
public:
	string srcAddr;
	string srcPort;
	string targetAddr;
	string targetPort;

	IComm* pIComm;

  // constructor accepts reference to Bridge as an ITask

	Task(ITask^ hBridge = nullptr);
  // Long task

  void makeSearchReq(std::string cstrNumThreads, std::string srchStr, std::string nReqs);
  void makeDownloadReq();
  void makeUploadReq(std::string dir);
  void makeConnectReq(std::string ip, std::string port, std::string cltPort);
  void makeDisconnectReq();

private:
  gcroot<ITask^> hBridge_; // reference to C++\CLI Bridge
};

#endif
