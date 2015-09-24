/////////////////////////////////////////////////////////////////////
//  e.cpp - Implements receiver functionalities of a peer to peer
// channel
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
////////////////////////////////////////////////////////////////////
#include "Receiver.h"

string Folder::uploadDir = "./UploadFiles/";
string Folder::downloadDir = "./DownloadFiles/";
//constructs instance associated with a socket and a message queue
ClientHandlerThread::ClientHandlerThread(Socket s, BlockingQueue<Message>& q) : s_(s), bq(q) {}

//start processing of messages on the queue
void ClientHandlerThread::start()
{
	t = new thread(&ClientHandlerThread::processMessages, this);

}

//Creates if not exists, directory to store received files
void ClientHandlerThread::createDirForFiles(){
	std::string content; 
	Folder folder; string uploadDir = folder.getUploadDir();
	if (!Directory::exists(uploadDir)){
		Directory::create(uploadDir);
	}
	string downloadDir = folder.getDownloadDir();
	if (!Directory::exists(downloadDir)){
		Directory::create(downloadDir);
	}
}
//handle messages of the queue and post to the receiver's queue for it to dispatch it appropriately. If a file write into a file. 
void ClientHandlerThread::processMessages(){
	try{
		createDirForFiles();
		std::string content;  Folder folder; string destDir = folder.getUploadDir();
		do{
			const unsigned int BLOCK_SIZE = 1024;
			Message  recvdMsg;
			std::string msg = s_.readLine();
			recvdMsg.parseMsg(msg);
			string content = recvdMsg.getHdr().getAttrib("msg"), targetComm = recvdMsg.getHdr().getTargetCommunicator();
			if (content == "Response for quit_sender"){
				bq.enQ(recvdMsg); 
				break;
			}
			if (content == ""){
				continue;
			}
			string srcFilePath = recvdMsg.getHdr().getAttrib("filename"), cmd = recvdMsg.getHdr().getCmd(), srcAddr = recvdMsg.getHdr().getSrcAddr(), srcPort = recvdMsg.getHdr().getSrcPort(), fileName = srcAddr + "_" + srcPort + "_" + Path::getName(srcFilePath), destFilePath;
			if (recvdMsg.getHdr().getAttrib("msg") == "download_reply"){
				 destDir = folder.getDownloadDir();
			}
			else{
				 destDir = folder.getUploadDir();
			}
			destFilePath = destDir + fileName;
			if (cmd == "send_file" || cmd == "send_file_begin"){
				writeToFile(recvdMsg, srcFilePath, destFilePath);
			}
			else if (cmd == "end_of_file"){
				Header hdr = recvdMsg.getHdr();
				hdr.setAttrib("filename", destFilePath);
				hdr.setAttrib("src_file_path", srcFilePath);
				recvdMsg.setHdr(hdr); 
				bq.enQ(recvdMsg);
			}
			else{
				Timer timer;
				long long  stop = timer.getCurrentTime();		
				Header hdr = recvdMsg.getHdr();
				hdr.setAttrib("stop_time", to_string(stop));
				recvdMsg.setHdr(hdr);
				bq.enQ(recvdMsg);
			}
		} while (true);
	}catch (exception e){
		sout << e.what();
	}
}

// Writes the message to destination file
void ClientHandlerThread::writeToFile(Message& recvdMsg, string srcFilePath, string destFilePath){
	try{
		string cmd = recvdMsg.getHdr().getCmd();
		size_t len = recvdMsg.getHdr().getContentLength();
		char* block = new char[len];
		if (s_.recvAll(block, len)){
			std::ofstream ofstream;
			if (cmd == "send_file"){
				ofstream.open(destFilePath.c_str(), std::ios::out | ios::app | ios::binary);
			}
			else{
				ofstream.open(destFilePath.c_str(), std::ios::out | ios::binary);
			}
			if (ofstream.good()){
				ofstream.write((char*)block, len);
				delete[] block;
				ofstream.close(); // clean up
			}
		}
		else{
			sout << "\n Recieved byte failed";
		}
		if (cmd == "send_file_begin"){
			string srcAddr = recvdMsg.getHdr().getSrcAddr(), srcPort = recvdMsg.getHdr().getSrcPort();
			string sMsg = "\n Started receiving file " + srcFilePath + " from " + srcAddr + ":" + srcPort + "\n\n";
			sout << sMsg;
		}
	}catch (exception e){
		sout << e.what();
	}
}

//wait till the thread finishes execution
void ClientHandlerThread::wait(){
	t->join();
}

// initialises listener on this port and spawns handler thread for incoming request
ListenThread::ListenThread(int port, BlockingQueue<Message>& q) : listener_(port), port_(port), q_(q), stop_(false), isClient(false){};

// initialises listener on this port and spawns handler thread for incoming request
ListenThread::ListenThread(int port, BlockingQueue<Message>& q, bool _isClient) : listener_(port), port_(port), q_(q), stop_(false), isClient(_isClient){};

//start listening for incoming connections on the port
void ListenThread::start()
{
	t = new thread(&ListenThread::processMessages, this);

}

//when a new request got, spawn handler thread for it
void ListenThread::processMessages()
{
	try{
		//CPU
		do{
			//sout << "\n Receiver : started listening again";
			SOCKET s = listener_.waitForConnect();
			/*string sMsg = "\nReceiver : Started Listening at " + port_;
			sout << sMsg ;
			*/
			ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
			pCh->start();
			clientThreadsVec.push_back(pCh);
		} while (!stop_ );
		//CPU
	}
	catch (exception e){
		sout << e.what();
	}

}

//wait for the client handlers to complete
void ListenThread::wait(){
	stop_ = true;
	for (auto th : clientThreadsVec){
		th->wait();
	}
}

// close listener
void ListenThread::close(){
	listener_.stop();
}

//Constructs the receiver instance
Receiver::Receiver(){
	dispatcher.setName("DispatcherCommunicator");
	dispatcher.start();
	start();
}

//register a communicator
void Receiver::registerComm(AbstractCommunicator* communicator){
	dispatcher.registerComm(communicator);
}

//connect to the port
void Receiver::connect(int port)
{
	try{
		pLt = new ListenThread(port, bq);
		string sMsg = "\n Receiver : Started listening at " + to_string(port) + "\n";
		sout << sMsg;

		pLt->start();
	}
	catch (std::exception ex){
		sout << ex.what();

	}
}


//connect to the port
void Receiver::connect(int port, bool isClient)
{
	try{
		pLt = new ListenThread(port, bq, isClient);
		string sMsg = "\n Receiver : Started listening at " + to_string(port) + "\n";
		sout << sMsg;

		pLt->start();
	}
	catch (std::exception ex){
		sout << ex.what();

	}
}
//process messages from the messaging queue and dispatch appropriately
void Receiver::processMessages(){
	try{
		while (true){
			Message  recvdMsg = bq.deQ();
			Header hdr = recvdMsg.getHdr();
			string content = hdr.getAttrib("msg");
			string targetComm = hdr.getTargetCommunicator();
			string cmd = recvdMsg.getHdr().getCmd();

			if (content == "Response for quit_sender"){
				sout << "\n quitting receiver";
				break;
			}

			if (content == ""){
				continue;
			}
			dispatcher.postMessage(recvdMsg);
		}
	}
	catch (std::exception ex){
		sout << ex.what();
	}
}

//start processing of messages
void Receiver::start(){
	t = new thread(&Receiver::processMessages, this);
}

//stop receivng messages
void Receiver::stop(){
	dispatcher.stop();
	dispatcher.wait();
	pLt->wait();
}

//wait for thread to complete processing
void Receiver::wait()
{
	t->join();

}

//destructor
Receiver::~Receiver(){}

//gets application wide upload dir
string Folder::getUploadDir(){
	return uploadDir;
}

//gets application wide download dir
string Folder::getDownloadDir(){
	return downloadDir;
}

#ifdef TEST_RCVR
#include "../Sender/Sender.h"
int main()
{
  int ret = 0;
  try
  {
	  Folder folder; string uploadDir = folder.getUploadDir();
	  sout << "\n upload dir:"<<uploadDir;
	string downloadDir = folder.getDownloadDir();
	  sout << "\n download dir:"<<downloadDir;


	Receiver rcvr;
	rcvr.connect(8181);
	EchoCommunicator echo;
	
	echo.setName("EchoCommunicator");
	rcvr.registerComm(&echo);
	echo.start();

	FileProcessingCommunicator fileComm;
	fileComm.setName("FileHandlerCommunicator");
	rcvr.registerComm(&fileComm);
	fileComm.start();
	string targetAddr = "127.0.0.1", targetPort = "8080";
	Sender sender;
	sender.setName("SenderCommunicator");
	////sender.connect(targetAddr, targetPort);
	rcvr.registerComm(&sender);
	sender.start();

	echo.wait();
	fileComm.wait();
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what();
    ret = 1;
  }
  catch(...)
  {
    sout << "\n  something bad happened";
    ret = 1;
  }
  sout << "\n\n";
  return ret;
}
#endif