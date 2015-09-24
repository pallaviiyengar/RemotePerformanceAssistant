/////////////////////////////////////////////////////////////////////
//  Communicator.cpp - Provides  concrete implementation of
// abstract communicator, namely EchoCommunicator and FilePRocessingCommunicator
//
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message Passing Communication, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "Communicator.h"
#include "../Sender/FileSystem.h"
#include "../Reciever/Receiver.h"

using namespace FileSystem;

// stop processing of messages
void ClientEchoCommunicator::stop(){
	Message msgStop;
	Header hdrStop;
	hdrStop.setAttrib("msg", "stop");
	hdrStop.setTargetCommunicator(this->name);
	msgStop.setHdr(hdrStop);
	this->postMessage(msgStop);
} 

//Get responses from the server and interpret accordingly
string ClientEchoCommunicator::getAndInterpretMessage(){
	string msg1_hdr = "";
	try{
		Message msg = bq.deQ();
		string content = msg.getHdr().getAttrib("msg");
		Header hdr = msg.getHdr();
		string addr = hdr.getAddress(), srcAddr = hdr.getSrcAddr(), srcPort = hdr.getSrcPort(), port = hdr.getPort();
		if (content == "download_reply"){
			string src_file = hdr.getAttrib("src_file_path");
			string filename = hdr.getAttrib("filename");
			FileInfo fileinfoDest(filename); FileInfo fileinfoSrc(src_file);
			string printMsg = "\n Received File " + src_file + " of SIZE = " + std::to_string(fileinfoSrc.size()) + " from " + srcAddr + ":" + srcPort + " and saved at " + filename + " with SIZE = " + std::to_string(fileinfoDest.size()) + " \n\n";
			sout << printMsg;
			return printMsg;
		}
		if (content == "Response for string_search"){
			string  time = hdr.getAttrib("time"), srchStr = hdr.getAttrib("search_string"), numFiles = hdr.getAttrib("num_of_files"), noOfThreads = hdr.getAttrib("no_of_threads"), start_time = hdr.getAttrib("start_time"), stop_time = hdr.getAttrib("stop_time");
			vector<string> files;
			int nFiles = atoi(numFiles.c_str());
			for (int i = 0; i < nFiles; i++){
				string name = "file" + to_string(i);
				string file = hdr.getAttrib(name);
				files.push_back(file);
			}
			Timer timer;
			long long  stop = atoll(stop_time.c_str()), start = atoll(start_time.c_str()), elapsedTime = stop - start;
			string oss = "";
			oss += "\n\nResponse for search string processing:";
			//oss += "\n Req ID: " + start_time;
			oss += "\n Request timestamp(time since epoch in miliseconds): " + start_time;
			oss += "\n Search string = " + srchStr;
			oss += "\n Num of threads used = " + noOfThreads;
			oss += "\n Turn-around time in mili seconds = " + to_string(elapsedTime);
			oss += "\n Server Processing time in mili seconds = " + time;
			oss += "\n Num of files string found in: " + to_string(nFiles);
			oss += "\n List of the files:";
			for (auto file : files){
				oss += "\n" + file;
			}
			return oss;
		}else{
			msg1_hdr = "\n"+ content + "\n";
		}
	}
	catch (exception e){
		sout << e.what();
	}
	return msg1_hdr;
}

//print out contents of the message
void ClientEchoCommunicator::processMessages(){
//	try{
//		while (true){
//			Message msg = bq.deQ();
//			string content = msg.getHdr().getAttrib("msg");
//			Header hdr = msg.getHdr();
//			string addr = hdr.getAddress(), srcAddr = hdr.getSrcAddr(), srcPort = hdr.getSrcPort(), port = hdr.getPort();
//			if (content == "download_reply"){
//				string src_file = hdr.getAttrib("src_file_path");
//				string filename = hdr.getAttrib("filename");
//				FileInfo fileinfoDest(filename); FileInfo fileinfoSrc(src_file);
//				string printMsg = "\n Received File " + src_file + " of SIZE = " + std::to_string(fileinfoSrc.size()) + " from " + srcAddr + ":" + srcPort + " and saved at " + filename + " with SIZE = " + std::to_string(fileinfoDest.size()) + " \n\n";
//				sout << printMsg;
//				continue;
//				//return printMsg;
//			}
//			if (content == "Response for string_search"){
//				string time = hdr.getAttrib("time"), srchStr = hdr.getAttrib("search_string"), numFiles = hdr.getAttrib("num_of_files"), noOfThreads = hdr.getAttrib("no_of_threads"), start_time = hdr.getAttrib("start_time"), stop_time = hdr.getAttrib("stop_time");
//				vector<string> files;
//				int nFiles = atoi(numFiles.c_str());
//				for (int i = 0; i < nFiles; i++){
//					string name = "file" + to_string(i);
//					string file = hdr.getAttrib(name);
//					files.push_back(file);
//				}
//				Timer timer;
//				long long  stop = atoll(stop_time.c_str()), start = atoll(start_time.c_str()), elapsedTime = stop - start;
//				string oss = "";
//				oss += "\n\nResponse for search string processing:";
//				oss += "\n Req ID: " + start_time;
//				oss += "\n Search string = " + srchStr;
//				oss += "\n Num of threads used = " + noOfThreads;
//				oss += "\n Turn-around time in mili seconds = " + to_string(elapsedTime);
//				oss += "\n Processing time in server in mili seconds = " + time;
//				oss += "\n List of files:";
//				for (auto file : files){
//					oss += "\n" + file;
//				}
//				sout << oss;
//				continue;
//				//return oss;
//			}
//			else{
//				//msg1_hdr = "\n\n Received below Header from " + srcAddr + ":" + srcPort + ":\n" + msg.getMsg()+ content + "\n";
//			}
//		}
//	}
//	catch (exception e){
//		sout << e.what();
//	}
}


// stop processing of messages
void EchoCommunicator::stop(){
	Message msgStop;
	Header hdrStop;
	hdrStop.setAttrib("msg", "stop");
	hdrStop.setTargetCommunicator(this->name);
	msgStop.setHdr(hdrStop);
	this->postMessage(msgStop);
}

//Process and print out contents of the message
void EchoCommunicator::processMessages(){
	try{
		while (true){
			Message msg = bq.deQ();
			string content = msg.getHdr().getAttrib("msg");
			Header hdr = msg.getHdr();
			string addr = hdr.getAddress();
			string srcAddr = hdr.getSrcAddr();
			string srcPort = hdr.getSrcPort();
			string port = hdr.getPort();
			if (content == "stop"){
				sout << "\n Stopping echo communicator\n";
				break;
			}
			if (content == "")
				continue;
			if (content == "download_reply"){
				string src_file = hdr.getAttrib("src_file_path");
				string filename = hdr.getAttrib("filename");		
				FileInfo fileinfoDest(filename); FileInfo fileinfoSrc(src_file);
				string printMsg = "\n Received File " + src_file + " of SIZE = " + std::to_string(fileinfoSrc.size()) + " from " + srcAddr + ":" + srcPort + " and saved at " + filename + " with SIZE = " + std::to_string(fileinfoDest.size()) + " \n\n";
				sout << printMsg;
				continue;
			}
			string resp = "Response for "; string respForFile = "Response for received file ";
			if (content.compare(0, resp.length(), "Response for ") != 0 && content.compare(0, respForFile.length(), "Response for received file ")!=0){
				AbstractDispatcher& dispatcher = AbstractDispatcher::getInstance();
				hdr.setTargetCommunicator("CLientEchoCommunicator");
				hdr.setAddr(srcAddr, srcPort);
				hdr.setSrcAddr(addr, port);
				hdr.setAttrib("msg", "Response for " + content);
				msg.setHdr(hdr);
				dispatcher.postMessage(msg);
			}
		}
	}
	catch (exception e){
		sout << e.what();
	}
}

//Stop file communicator processing
void FileProcessingCommunicator::stop(){
	Message msgStop;
	Header hdrStop;
	hdrStop.setAttrib("msg", "stop");
	hdrStop.setTargetCommunicator(this->name);
	msgStop.setHdr(hdrStop);
	this->postMessage(msgStop);
}

// process messages, send back a reply when a file successfully received
void FileProcessingCommunicator::processMessages(){
	try{
		while (true){
			Message msg = bq.deQ(); Header hdr = msg.getHdr();
			string addr = hdr.getAddress(), srcAddr = hdr.getSrcAddr(), srcPort = hdr.getSrcPort(), port = hdr.getPort(), cmd = hdr.getCmd(), content = hdr.getAttrib("filename");
			AbstractDispatcher& dispatcher = AbstractDispatcher::getInstance();
			Message msgReply; Header hdrReply;
			hdrReply.setAddr(srcAddr, srcPort);
			hdrReply.setSrcAddr(addr, port);
			if (content == "stop"){
				sout << "\n Stopping file processing communicator \n";
				break;
			}
			if (cmd == "get_file"){//download
				Folder folder; string destDir = folder.getUploadDir(); FileManager mgr;
				std::vector<std::string> files = mgr.searchFiles(destDir, "*.*", false);
				//Send all files in the upload directory
				for (string path : files){
					hdrReply.setTargetCommunicator("CLientEchoCommunicator");
					hdrReply.setCmd("send_file");
					hdrReply.setAttrib("filename", path);
					hdrReply.setAttrib("msg", "download_reply");
					msgReply.setHdr(hdrReply);
					dispatcher.postMessage(msgReply);
				}

			}
			else{//upload
				string src_file = hdr.getAttrib("src_file_path");
				FileInfo fileinfoDest(content); FileInfo fileinfoSrc(src_file); 				//string cmd = hdr.getAttrib("send_file_begin");		
				string printMsg = "\n Received File " + src_file + " of SIZE = " + std::to_string(fileinfoSrc.size()) + " from " + srcAddr + ":" + srcPort + " and saved at " + content + " with SIZE = " + std::to_string(fileinfoDest.size()) + " \n\n";
				sout << printMsg;
				hdrReply.setTargetCommunicator("CLientEchoCommunicator");
				string strMsg = "Response for received file " + Path::getName(src_file);
				hdrReply.setAttrib("msg", strMsg);
				hdrReply.setCmd("echo_msg");
				msgReply.setHdr(hdrReply);
				dispatcher.postMessage(msgReply);

			}
		}
	}catch (exception e){
		sout << e.what();
	}
}

// String search processing on specified directory on specified string and specified number of threads
void StringSearchCommunicator::processMessages(){
	try{
		while (true){
			Message msg = bq.deQ();
			StringProcessor* sp = new StringProcessor(msg);
			sp->start();
			strProcessors.push_back(sp);
		}
	}
	catch (exception e){
		sout << e.what();
	}
}

// Stop string search processing
//void StringSearchCommunicator::stop(){}

// Initialisation of instance of result of search string processing
SearchResult::SearchResult(string file) : found(false){ _file = file; }

//If search string on the associated file was found
bool SearchResult::isFound(){
	return found;
}

//Get file associated with the string search
string SearchResult::getFile(){
	return _file;
}

// Set if search string was found
void SearchResult::setFound(bool fnd){
	found = fnd;
}

//String processing instance constructor
StringProcessor::StringProcessor(Message msg){
	try{
		request = msg;
		/*Timer timer;
		long long time_stamp = timer.getCurrentTime();
		request.getHdr().setAttrib("start_time", to_string(time_stamp));*/

		Header hdr = msg.getHdr();
		//srchDir = hdr.getAttrib("directory");
		Folder folder;
		srchDir = folder.getUploadDir();
		srchString = hdr.getAttrib("search_string");
		numThreads = atoi(hdr.getAttrib("no_of_threads").c_str());
		if (numThreads == 0){
			numThreads = 1;
		}
		FileManager mgr;
		std::vector<std::string> files = mgr.searchFiles(srchDir, "*.*", false);

		numFiles = 0;
		std::string exclude_exts[] = {  "zip", "exe", "jpg", "jpeg", "png", "JPG", "JPEG", "EXE" };//exclude extendions for string search

		for (auto file : files){
			std::string extension = Path::getExt(file);
			bool isSkip = false;
			for (auto ext : exclude_exts){
				if (ext == extension){
					isSkip = true;
					break;
				}
			}
			if (isSkip){
				continue;
			}
			SearchResult sr(file);
			bqIn.enQ(sr);
			numFiles++;
		}
	}
	catch (exception e){ sout << e.what(); }
}

//Start string search spawning number of threads specified
void StringProcessor::start(){
	try{
		if (numFiles == 0){
			constructResponse();

			return;
		}
		//psthread = new thread(&StringProcessor::processMessages, this);
		thread(&StringProcessor::processMessages, this).detach();
		for (int i = 0; i < numThreads; i++){
			thread(&StringProcessor::search, this).detach();

			//threads.push_back(new thread(&StringProcessor::search, this));
		}
	}catch (exception e){
		sout<<e.what();
	}
}

//Read contents of the file as a string
string StringProcessor::readContents(string file){
	string content = "";
	try{
		ifstream ins(file, ios::in | ios::binary);
		if (ins){
			ostringstream oss;
			oss << ins.rdbuf();
			ins.close();
			content = oss.str();
		}
	}catch (exception e){
		sout << e.what();
	}
	return content;
}

//Search for string in specified file
bool StringProcessor::searchStr(string file, string str){
	bool fnd = false;
	try{
		string content = readContents(file);
		int pos = content.find(str);
		if (pos != string::npos){
			fnd = true;
		}
	}catch (exception e){
		sout << e.what();
	}
	return fnd;
}

//Processing that goes on in each thread
void StringProcessor::search(){
	try{
		ostringstream ossStart;
		ossStart << "\n Started Thread ID : " << std::this_thread::get_id() << " \n";
		sout << ossStart.str();

		while (true){
			if (bqIn.isEmpty()){
				ostringstream ossEnd;
				ossEnd << "\n Exiting from Thread ID : " << std::this_thread::get_id() << " \n";
				sout << ossEnd.str();

				return;
			}
			SearchResult sr = bqIn.deQ();
			ostringstream ossProcess;
			ossProcess << "\n Processing in Thread ID : " << std::this_thread::get_id() << " file " << sr.getFile() << " for string " << srchString << "\n";
			sout << ossProcess.str();
			if (searchStr(sr.getFile(), srchString)){
				sr.setFound(true);
			}
			else{
				sr.setFound(false);
			}
			bqOut.enQ(sr);
			if (sr.isFound()){
				thread::id tid = (std::this_thread::get_id());
				stringstream ss;
				ss << tid;
				string msg = "\nIn  thread " + ss.str() + " found \"" + srchString + "\" in file " + sr.getFile() + "\n";
				sout << msg;
				//filesFoundIn.push_back(sr.getFile());
			}
		}
	}catch (exception e){
		sout << e.what();
	}
}

//Processing the results of search done by each thread and constructing  a response
void StringProcessor::processMessages(){
	sout << "\nIn string processor for string : " << srchString;

	try{
		int j = 0;
		Timer timer; timer.start();

		while (true){
			SearchResult sr = bqOut.deQ();
			if (sr.isFound()){
				filesFoundIn.push_back(sr.getFile());
			}
			//construct response
			j++;
			if (j == numFiles){
				//sout << "\nexiting from string processor";
				break;
			}
			/*for (std::thread* thrd : threads){
				thrd->join();
			}*/
			/*if (bqOut.isEmpty()){
				sout << "\nExiting from string processor\n";
				break;
				}*/
		}
		timer.stop();
		elapsedTime  = timer.elapsedTime<std::chrono::milliseconds>();
		constructResponse();

	}catch (exception e){
		sout << e.what();
	}
}

//Construct response for string search operation
void StringProcessor::constructResponse(){
	Message msg = request; 	Header hdr = msg.getHdr();
	string content = msg.getHdr().getAttrib("msg");
	string addr = hdr.getAddress();
	string port = hdr.getPort();
	string srcAddr = hdr.getSrcAddr();
	string srcPort = hdr.getSrcPort();

	Message msgReply; Header hdrReply;
	hdrReply = hdr;
	hdrReply.setAddr(srcAddr, srcPort);
	hdrReply.setSrcAddr(addr, port);
	hdrReply.setTargetCommunicator("CLientEchoCommunicator");	
	hdrReply.setAttrib("msg", "Response for " + content);
	hdrReply.setAttrib("time", to_string(elapsedTime));
	hdrReply.setAttrib("num_of_files", to_string(filesFoundIn.size()));
	hdrReply.setAttrib("search_string", srchString);
	for (unsigned int i = 0; i < filesFoundIn.size();i++){
		string name = "file" + to_string(i);
		hdrReply.setAttrib(name, filesFoundIn[i]);
	}
	msgReply.setHdr(hdrReply);
	AbstractDispatcher& dispatcher = AbstractDispatcher::getInstance();
	dispatcher.postMessage(msgReply);
}

//Test stub
#ifdef TEST_COMMUNICATOR
#include "../Sender/Sender.h"
int main()
{
	int ret = 0;
	try
	{
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
		rcvr.registerComm(&sender);
		sender.start();

		StringSearchCommunicator stringSearchComm;
		stringSearchComm.setName("StringSearchCommunicator");
		rcvr.registerComm(&stringSearchComm);
		stringSearchComm.start();

		echo.wait();
		fileComm.wait();
		echo.stop();
		fileComm.stop();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what();
		ret = 1;
	}
	catch (...)
	{
		sout << "\n  something bad happened";
		ret = 1;
	}
	sout << "\n\n";
	return ret;
}
#endif