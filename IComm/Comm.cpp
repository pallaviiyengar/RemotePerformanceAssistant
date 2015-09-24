/////////////////////////////////////////////////////////////////////
//  Comm.cpp - Provides  concrete implementation for
// IComm interface and wrapper for core functionalities thus hiding C+11 threading functionalitites from client.
// Any client application has to just use IComm.dll and
// Icomm.h.
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#define IN_DLL
#include "Comm.h"

//Creates instance of concrete Comm class used by other applications, like a C++/CLI application 
IComm* IComm::Create(){ return new Comm(); }

//Constructs instance
Comm::Comm()
{}

//set source address and port
void  Comm::setSrcAddr(string _srcAddr, string _srcPort){
	srcAddr = _srcAddr; srcPort = _srcPort;
}

//Start client
void Comm::start(){
	rcvr.connect(atoi(srcPort.c_str()), true);//Receiver started
	echo.setName("CLientEchoCommunicator");
	rcvr.registerComm(&echo);
	echo.start();//Echo communicator started

	sndr.start();//sender started
}

//Start client
void Comm::stop(string targetAddr, string targetPort){
	Header hdr;
	Message msgMsg;
	hdr.setCmd("echo_msg");
	hdr.setAttrib("msg", "quit_sender");
	hdr.setAddr(targetAddr, targetPort);
	hdr.setSrcAddr(srcAddr, srcPort);
	hdr.setTargetCommunicator("EchoCommunicator");
	msgMsg.setHdr(hdr);
	sndr.postMessage(msgMsg);
}

//Get response from server
string Comm::getEchoMessage(){
	return echo.getAndInterpretMessage();
}

//Send plain text to server
void Comm::postEchoMessage(string targetAddr, string targetPort, string message){
	Header hdr; Message msgMsg;
	hdr.setCmd("echo_msg"); 
	hdr.setAttrib("msg", message);
	hdr.setAddr(targetAddr, targetPort); 
	hdr.setSrcAddr(srcAddr, srcPort);
	hdr.setTargetCommunicator("EchoCommunicator");
	msgMsg.setHdr(hdr);
	sndr.postMessage(msgMsg);
}

//Making upload request to the server
int Comm::upload(string targetAddr, string targetPort, string dir){
	int size = 0;
	try{
		FileManager mgr;
		std::vector<std::string> files = mgr.searchFiles(dir, "*.*", false);
		if (files.size()==0){
			return 0;
		}
		size = files.size();
		for (auto file : files){
			Header hdrFile2; Message msgFile2;
			hdrFile2.setCmd("send_file");
			hdrFile2.setAddr(targetAddr, targetPort);//IP address
			hdrFile2.setAttrib("filename", file);//name value pairs
			hdrFile2.setAttrib("msg", "File transfer");
			hdrFile2.setSrcAddr(srcAddr, srcPort);
			hdrFile2.setContentLength(1024);
			hdrFile2.setTargetCommunicator("FileHandlerCommunicator");
			msgFile2.setHdr(hdrFile2);
			sndr.postMessage(msgFile2);
		}
	}
	catch (exception e){
		sout<<e.what();
		return 0;
	}
	return size;
}

//Making download request to the server
void Comm::download(string targetAddr, string targetPort, string downFile){
	try{
		Header hdrFile2;
		Message msgFile2;
		hdrFile2.setCmd("get_file");
		hdrFile2.setAddr(targetAddr, targetPort);//IP address
		hdrFile2.setAttrib("filename", downFile);//name value pairs
		hdrFile2.setAttrib("msg", "download file");
		hdrFile2.setSrcAddr(srcAddr, srcPort);
		hdrFile2.setContentLength(1024);
		hdrFile2.setTargetCommunicator("FileHandlerCommunicator");
		msgFile2.setHdr(hdrFile2);
		sndr.postMessage(msgFile2);
	}
	catch (exception e){
		sout << e.what();
	}

}

//Making search string request on specified directory with specified search string on specified number of threads
void Comm::searchString(string targetAddr, string targetPort, string targetDir, string str, string noOfThreads, string nReqs){
	try{
		int numReqs = atoi(nReqs.c_str()); 
		if (numReqs == 0){
			numReqs = 1;
		}

		for (int i = 0; i < numReqs; i++){
			Header hdrFile3; Message msgFile3;
			hdrFile3.setAddr(targetAddr, targetPort);//IP address
			hdrFile3.setSrcAddr(srcAddr, srcPort);
			hdrFile3.setTargetCommunicator("StringSearchCommunicator");
			hdrFile3.setAttrib("directory", targetDir);
			hdrFile3.setAttrib("search_string", str);
			hdrFile3.setAttrib("no_of_threads", noOfThreads);
			hdrFile3.setCmd("search_string");
			hdrFile3.setAttrib("msg", "string_search");
			Timer timer;
			string time_stamp = std::to_string(timer.getCurrentTime());
			hdrFile3.setAttrib("start_time", time_stamp);
			msgFile3.setHdr(hdrFile3);
			sndr.postMessage(msgFile3);
		}
	}
	catch (exception e){
		sout << e.what();
	}
}

//Wait for the communicators to end processing
void Comm::wait(){
	//echo.wait();
	//sndr.wait();
	rcvr.wait();
	//sout << "\n receiver wait exit";

	sndr.stop();
	//sout << "\n sender stopped";

	sndr.wait();
	//sout << "\n sender wait exit";
}

//Test stub
#ifdef TEST_I_COMM
int main(){
	string srcAddr = "127.0.0.1", srcPort = "8080", targetAddr = "127.0.0.1", targetPort = "8181", dir = ".\\UploadFiles", srchStr = "hi", numThreads = 3, numReqs = 3;

	cout<<"----Demmonstration communication interface -----";
	IComm* pIComm = IComm::Create();
	pIComm->setSrcAddr(srcAddr, srcPort);
	cout<<"----Starting client-----";
	pIComm->start();
	cout<<"----Post string message-----";
	pIComm->postEchoMessage(targetAddr, targetPort, "hello");
	cout<<"----get response-----";
	string echo = pIComm->getEchoMessage();
	cout << "\n Received message is :" << echo;

	cout<<"----Demmonstration upload of files -----";
	pIComm->upload(targetAddr, targetPort, dir);

	cout<<"----Demmonstration download of files -----";
	pIComm->download(targetAddr, targetPort, "127.0.0.1_8484_zzzzimage.jpg");

	cout<<"----Demmonstration searching string -----";

	for (int i = 0; i < atoi(numReqs.c_str()); i++){
		pIComm->searchString(targetAddr, targetPort, dir, srchStr, numThreads);
	}
	for (int i = 0; i < atoi(numReqs.c_str()); i++){
		sout << "\n" << pIComm->getEchoMessage();
	}

	pIComm->stop(targetAddr, targetPort);

	pIComm->wait();
}
#endif