/////////////////////////////////////////////////////////////////////
//  Executive.cpp - Provides executive class that runs a server 
//  instance
//
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////

#include "../Sender/Sender.h"
#include "../Reciever/Receiver.h"
#include "../ClientExecutive/Display.h"
int main(int argc, char* argv[]){
	int ret = 0;
	try
	{
		Display disp;
		if (argc < 2){
			disp.printServerUsage();
			return 1;
		}
		int port = atoi(argv[1]);
		disp.printInfo("server", argv[1]);

		Receiver rcvr;
		rcvr.connect(port);//Receiver and dispatcher started

		EchoCommunicator echo;
		echo.setName("EchoCommunicator");
		rcvr.registerComm(&echo);
		echo.start();

		FileProcessingCommunicator fileComm;
		fileComm.setName("FileHandlerCommunicator");
		rcvr.registerComm(&fileComm);
		fileComm.start();

	
	//CPU

		
		Sender sender;
		sender.setName("SenderCommunicator");
		rcvr.registerComm(&sender);
		sender.start();


	//CPU

		//Commented PAllavi
		StringSearchCommunicator stringSearchComm;
		stringSearchComm.setName("StringSearchCommunicator");
		rcvr.registerComm(&stringSearchComm);
		stringSearchComm.start();

		echo.wait();
		fileComm.wait();
	//CPU

		sender.wait();
		rcvr.wait();

		

	}
	catch (std::exception& ex){
		std::cout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch (...){
		sout << "\n  something bad happened";
		ret = 1;
	}
	sout << "\n\n";
	return ret;
}