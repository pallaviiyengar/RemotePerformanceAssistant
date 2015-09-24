/////////////////////////////////////////////////////////////////////
//  Executive.cpp - Provides  client executive class that runs the client
//
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message passing, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "../Reciever/Receiver.h"
#include "../Sender/Sender.h"
#include "../Sender/Header.h"
#include "../Sender/Message.h"
#include "FileManager.h"
#include "Display.h"
#include "../IComm/Comm.h"

//Starts up the client, reads cmd line args, and dispatches to server accordingly. Also, starts up required communicator and receiver. 
int main(int argc, char* argv[]){
	int ret = 0; vector<string> msgs; Display disp;
	try{
		if (argc < 5){
			disp.printClientUsage();
			return 1;
		}

		string srcAddr = "127.0.0.1", srcPort = argv[1], targetAddr = argv[2], targetPort = argv[3], opt = argv[4];
		//srchStr = argv[5], numThreads = argv[6], numReqs = argv[7];
		sout << "\n1: " << argv[0] << " 2: " << argv[1] << " 3: " << argv[2] << " 4: " << argv[3] << " 5: " << argv[4];
		//<< " 6 srch string: " << argv[5] << " 7 Num Threads: " << argv[6] << " 8 num reqs: " << argv[7];
		/*if (argc < 5){
			disp.printClientUsage();
			return 1;
		}*/
		string inputDir = "", srchStr = "", numThreads = "1", numReqs = "1";
		Folder folder; string dir = folder.getUploadDir();

		IComm* pIComm = IComm::Create();
		pIComm->setSrcAddr(srcAddr, srcPort);
		pIComm->start();
		 if (opt=="/u"){
			if (argc<6){
				disp.printClientUsage();
				return 1;
			}
			inputDir = argv[5];
			int upsize;
			//Constructing messages of files to be uploaded
			if ((upsize = pIComm->upload(targetAddr, targetPort, inputDir))>0){
				for (int i = 0; i < upsize;i++){
					sout << "\n" << pIComm->getEchoMessage();
				}
			}
			else{
				sout << "\nCould not upload. Check the upload directory\n";
			}

		 }
		else if (opt == "/s"){
			if (argc<6){
				disp.printClientUsage();
				return 1;
			}
			srchStr = argv[5];
			if (argc>6){
				numThreads = argv[6];
			}
			if (argc>7){
				numReqs = argv[7];
			}

			dir = "";
		
			pIComm->searchString(targetAddr, targetPort, dir, srchStr, numThreads, numReqs);

			for (int i = 0; i < atoi(numReqs.c_str()); i++){
				sout << "\n" << pIComm->getEchoMessage();
			}

		}
		else if (opt=="/d"){
			pIComm->download(targetAddr, targetPort, "foobar.txt");
			sout << "\n" << pIComm->getEchoMessage();

		}
		else{
			disp.printClientUsage();
		}
	
		pIComm->stop(targetAddr, targetPort);

		pIComm->wait();
		
		system("pause");
	}
	catch (std::exception& ex){
		sout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch (...){
		sout << "\n\n  something bad happend to a sender";
		ret = 1;
	}
	return ret;
}
