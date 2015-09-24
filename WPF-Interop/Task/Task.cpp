/////////////////////////////////////////////////////////////////////
// Task.cpp - Implementation of long running Task  //
//         
// Author: Pallavi
// Source:  Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014       //
/////////////////////////////////////////////////////////////////////
/*
*  C++\CLI code will not compile if it attempts to use a C++11 thread.
*  You need to isolate the threaded code behind an interface and
*  object factory in native C++ (don't compile with /CLR option).
*
*  Then package that component as a DLL and have the C++\CLI code make
*  a reference to the DLL.  Now it will compile and run.
*
*  This code shows you how to do that.  Compile the C++\CLI code with
*  the /clr option set in the General Project Properties.  Make sure
*  that it has a reference to the native DLL and includes the DLL Lib file.
*
*  Use the dll protocol shown in Comm.cpp and compile without /CLR option.
*  Now you can build and run the project.
*/

#include <string>
#include "Task.h"
#include "ITask.h"
#include "../../IComm/Icomm.h"
#include <iostream>

using namespace System;
using namespace System::Text;

String^ convert(const std::string& s)
{
	StringBuilder^ sb = gcnew StringBuilder();
	for (char ch : s)
		sb->Append(static_cast<wchar_t>(ch));
	return sb->ToString();
}

std::string convert(String^ s)
{
	std::string temp;
	for (int i = 0; i < s->Length; ++i)
		temp += static_cast<char>(s[i]);
	return temp;
}

Task::Task(ITask^ hBridge) : hBridge_(hBridge) {
	pIComm = IComm::Create();
}

void Task::makeConnectReq(std::string ip, std::string port, std::string cltPort){
	srcAddr = "127.0.0.1", srcPort = cltPort, targetAddr = ip, targetPort = port;
	pIComm->setSrcAddr(srcAddr, srcPort);
	pIComm->start();
	//isConnected = true;
	string result = "connect";
	if (static_cast<ITask^>(hBridge_) != nullptr)  // did Bridge register itself?
		hBridge_->doCallBack("connected");
}

void Task::makeSearchReq(std::string cstrNumThreads, std::string srchStr, std::string nReqs){
	try{
		string dir = "../UploadFiles/";

		pIComm->searchString(targetAddr, targetPort, dir, srchStr, cstrNumThreads, nReqs);
		int iReqs = atoi(nReqs.c_str());
		string echo="";
		if (iReqs == 0){
			iReqs = 1;
		}
		for (int i = 0; i < iReqs; i++)
		{
			echo += pIComm->getEchoMessage();
		}

		if (static_cast<ITask^>(hBridge_) != nullptr){  // did Bridge register itself?
			hBridge_->doCallBack(echo);
		}
	}
	catch (exception e){
		e.what();
	}
}

void Task::makeDisconnectReq(){
	try{
		if (pIComm!=NULL){
			pIComm->stop(targetAddr, targetPort);
		}
		if (static_cast<ITask^>(hBridge_) != nullptr){  // did Bridge register itself?
			hBridge_->doCallBack("Disconnected");
		}
	}
	catch (exception e){
		e.what();
	}
}
#ifdef TEST_TASK
int main(array<System::String ^> ^args)
{
	Console::Write("\n  C++\\CLI client using native C++ DLL with C++11 thread");
	Console::Write("\n =======================================================\n");


	string srcAddr = "127.0.0.1", srcPort = "8484", targetAddr = "127.0.0.1", targetPort = "8181";
	 Task t;

	 Console::Write("\n  Demonstarating connecting to server");
	 Console::Write("\n =======================================================\n");

	 t.makeConnectReq(targetAddr, targetPort, srcPort);

	 Console::Write("\n  Demonstarating posting search operation to server");
	 Console::Write("\n =======================================================\n");

	 std::string nThreads = "3", nReqs = "3";
	 t.makeSearchReq(nThreads, "hi", nReqs);

	Console::Write("\n\n");
	return 0;
}
#endif