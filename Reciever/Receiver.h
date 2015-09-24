/////////////////////////////////////////////////////////////////
// Reciever.h - reciever class with concurrent clients that 
//				handle incoming messages appropriately whether a file
//				or a plain text.
//				The received messages are handeled appropriately and 
//				posted to the dispatcher which in turn hands them over to
//              the appropriate communicator
//   
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:	   Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message passing, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar				 
//  Source:		   Jim Fawcett
/////////////////////////////////////////////////////////////////
/*
* Module operation:
* This class receives, interprets and handles appropriately incoming messages, be it a file or a plain text message.
*
* Follows HTTP like protocols, with lines in the header and bytes in the body, if there is one,
* specified by a header line something like:
*    content_length : 1024
*
* If a file message is received, the header indicates the length of the body and
* so many blocks are fetched and written to a file and this continues until all blocks are received and
* then a message is posted to the dispatcher indicating details of the file received, so that it can
* to be handled appropriately.
* 
*If it is a plain text message, it is dispatched as it is to the dispatcher.
*
* This package has following classes:
* Class Receiver:
* - Dispatches processed message to the dispatcher.
*
* - public interface:
*   Receiver rcvr;
*   rcvr.connect(8181);
*	EchoCommunicator echo;
*	echo.setName(COMM_ECHO);
*	rcvr.registerComm(&echo);
*	echo.start();
*   rcvr.stop();
*   rcvr.wait();
*
* Class Listenerthread
*	-Listens and creates new thread for processing request for each new incoming socket requests on the receiving port
*	-public interface
*		int port;
*		BlockingQueue bq;
*       ListenThread* pLt = new ListenThread(port, bq);
*		pLt->start();
*		pLt->stop();
*		pLt->wait();
*
* Class ClientHandlerThread
*	-Handles incoming messages of a socket appropriately depending on if a file or an echo message is received
*	-Public interface:
*		Socket s; BlockingQueue q_;
*   	ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
*		pCh->start();
*       pCh->wait();
*
*Class folder
*-Holds location of upload and downlaod directories
*Public interface:
*Folder folder;
*folder.getUploadDir();
*folder.getDownloadDir();
*
* Required files:
* - Sockets.h, Sockets.cpp, BlockingQueue.h, BlockingQueue.cpp
*   AbstractCommunicator.h, AbstractCommunicator.cpp
*   Message.h, Message.cpp. Header.h
*   BlockingQueue.h, BlockingQueue.cpp
*   Locks.h, Locks.cpp
*   FileSystem.h, FileSystem.cpp, FileManager.h, FileManager.cpp
*   Communicator.h, Communicator.cpp
*
*Build Process:
*- devenv MessagePassing.sln
*
* Maintanence History:
* ver 1.1 Apr18 2014,
* Changed ClientHandlerThread for download of files
* Added folder class to hold location of upload and downlaod directories
*
* ver 1.0 - 10 Apr 2014
* - first release
*/
#ifndef RECEIVER_H
#define RECEIVER_H
#include "../Sockets/Sockets.h"
#include <thread>
#include "../Threads/Locks.h"
#include "../BlockingQueue/BlockingQueue.h"
#include <string>
#include "../Sender/Message.h"
#include "../Sender/Header.h"
#include "../Sender/FileSystem.h"
#include "../Communicator/Communicator.h"
#include "../Communicator/AbstractCommunicator.h"
#include "../ChronoTimer/ChronoTimer/ChronoTimer.h"

using namespace FileSystem;

///////////////////////////////////////////////////
// ClientHandlerThread handles incoming messages associated with a socket
class ClientHandlerThread
{
public:
	//constructs instance associated with a socket and a message queue
	ClientHandlerThread(Socket s, BlockingQueue<Message>& q);

	//start processing of messages on the queue
	void start();

	//handle messages of the queue and post to the receiver's queue for it to dispatch appropriately. If a file is received,
	// it is written and stored at a pre defined location. 
	void processMessages();

	//wait till the thread finishes execution
	void wait();


private:
	Socket s_;
	BlockingQueue<Message>& bq;
	std::thread* t;

	void createDirForFiles();
	void writeToFile(Message& recvdMsg, string srcFilePath, string destFilePath);

};

///////////////////////////////////////////////////
// listenThread that listens on a specified port for incoming connection requests
class ListenThread 
{
	public:
	//initialises listener on this port and spawns handler thread for incoming request
	ListenThread(int port, BlockingQueue<Message>& q);
	
	//initialises listener on this port and spawns handler thread for incoming request, also specify if server or client
	ListenThread(int port, BlockingQueue<Message>& q, bool _isClient);

	//start listening for incoming connections on the port
	void start();

	//when a new request got, spawn handler thread for it
	void processMessages();

	//wait for the processing to complete
	void wait();
	
	// close listener
	void close();

private:
	bool stop_, isClient;
	BlockingQueue<Message>& q_;
	SocketListener listener_;
	int port_;
	vector<ClientHandlerThread*> clientThreadsVec;
	std::thread* t;
	vector<Socket> socketArray;
};

class Receiver{
public:
	//Constructs the receiver instance
	Receiver();

	//register a communicator
	void registerComm(AbstractCommunicator* communicator);

	//connect to the port
	void connect(int port);

	//connect to port and specify if server or client
	void connect(int port, bool isClient);

	//start processing of messages
	void start();

	//stop receivng messages
	void stop();

   //wait for thread to complete processing
	void wait();

   //destructor
	virtual ~Receiver();

private:
	MessageDispatcher dispatcher;
	std::thread* t;
	BlockingQueue<Message> bq;
	ListenThread* pLt;

	//process messages from the messaging queue and dispatch appropriately
	void processMessages();
};

//Defines server location of upload and download files
class Folder{
private:
	static string uploadDir;
	static string downloadDir;

public:
	string getUploadDir();
	string getDownloadDir();
};
#endif