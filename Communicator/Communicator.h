/////////////////////////////////////////////////////////////////////
//  Communicator.h -  This class provides concrete classes deriving
// from AbstractCommunicator that process messages 
//																   // 
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
*  EchoCommunicator:
*    Simply diplays its messages on the console and posts back a reply
*
*  FileProcessingCommunicator:
*    Posts reply back indicating a file has been received
*	Also, handles download of files
*
* MessageDispatcher: Concrete implementation of  AbstractDispatcher, which dispatches to the target communicator
* specified in the messages
*
*Client Echo Communictor:
*Echo communicator on the client side that receives each response from server
*
*SearchResult:
*Result type of search processing associated with each file and says if or not the search string was found.
*
*StringProcessor:
*The class that handles each search processing request by spawning specified number of threads and, later 
*combining the results, and sending repsonse
*
*StringSearchCommunicator:
*The communicator class which spawns string processor for each incoming request
*
*
Public Interface:
=================
EchoCommunicator echo;
echo.setName("COMM_ECHO");
string name = echo.getName();
echo.start();
Message msg;
echo.postMessage(msg);
Message msg = echo.getMessage();
echo.stop();
echo.wait();

FileProcessingCommunicator fileComm;
fileComm.setName("FileHandlerCommunicator");
string name = fileComm.getName();
fileComm.start();
Message msg;
fileComm.postMessage(msg);
Message msg = fileComm.getMessage();
fileComm.stop();
fileComm.wait();

MessageDispatcher msgDispatcher;
Message message;
msgDispatcher.post(Msg);

StringSearchCommunicator stringSearchComm;
stringSearchComm.setName("StringSearchCommunicator");
stringSearchComm.start();

StringProcessor* sp = new StringProcessor(msg);
sp->start();

string file="my.txt";
SearchResult sr(file);
bool found = sr.isFound();
string filename sr.getFile();

Build Process:
==============
Required files
AbstractCommunicator.h, Message.h

Build commands
==============
- devenv MessagePassing.sln

Maintenance History:
====================
* ver 1.2 Apr26, 2014
* Added string searching processing functionality with specified number of threads
* ver 1.1 Apr18 2014,
* Changed FileCommunicator to handle download of files
*
ver 1.0 : 10 Apr 2014
- first release

*/
//
#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include "../Communicator/AbstractCommunicator.h"
#include "../Sender/Message.h"
#include "../Sender/FileSystem.h"
#include "../Threads/Locks.h"
#include <string>
#include "../ClientExecutive/FileManager.h"
#include "../ChronoTimer/ChronoTimer/ChronoTimer.h"
using namespace std;
using namespace FileSystem;

/* EchoCommunicator:
* Diplays its messages on the console and posts back a reply
*/
class EchoCommunicator : public AbstractCommunicator
{
private:
	//print out contents of the message
	void processMessages();

public:
	// stop processing of messages
	void stop();
};

/* ClientEchoCommunicator:
* Receives responses from server, interprets and returns in form of string
*/
class ClientEchoCommunicator : public AbstractCommunicator
{
private:
	//print out contents of the message
	void processMessages();

public:
	// stop processing of messages
	void stop();

	// interprets and print out contents of the message as a string
	string getAndInterpretMessage();
};

/*Posts reply back indicating a file has been received */ 
class FileProcessingCommunicator : public AbstractCommunicator{
private:
	// process messages, send back a reply when a file successfully received
	void processMessages();

public:
	//stop processing of messages
	void stop();
};


//String search result containing file and if search string found or not
class SearchResult{
private:
	bool found;
	string _file;
public:
	SearchResult(string file);
	bool isFound();
	string getFile();
	void setFound(bool isFound);
};

//Provides string search processing on a directory with specified search string on specified number of threads
class StringProcessor{
private:
	BlockingQueue<SearchResult> bqIn;
	BlockingQueue<SearchResult> bqOut;
	string srchDir;
	string srchString;
	int numThreads;
	int numFiles;
	//vector<thread*> threads;
	//thread* psthread;
	bool searchStr(string file, string str);
	string readContents(string file);
	void search();
	Message request;
	vector<string> filesFoundIn;
	long long elapsedTime;
	void constructResponse();
	void processMessages();

public:
	StringProcessor(Message msg);

	void start();
};


//Searches for string in a set of files
class StringSearchCommunicator : public AbstractCommunicator{
private:
	vector<StringProcessor*> strProcessors;

	// process messages, send back a reply when a file successfully received
	void processMessages();

public:
	//stop processing of messages
	//void stop();
};

//Concrete implementation of dispatcher. currently has no specialised implementation
class MessageDispatcher : public AbstractDispatcher{};

#endif
