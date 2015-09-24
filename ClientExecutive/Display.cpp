/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display class implementation which prints useful general info about the
// calling executive class
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message passing, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "Display.h"

//constructor instance
Display::Display()
{
}

//prints client usage
void Display::printClientUsage()
{
	string usage = "\n\nClient usage:"
		"\n ClientExecutive LISTENING_PORT SERVER_IP/SERVER_NETWORK_NAME TARGET_PORT OPTIONS[/u dir, /d, /s srch_str nThreads nRequests]"
		"\n 1. LISTENING_PORT - Receives incoming connections including responses from the server "
		"\n 2. SERVER_IP/SERVER_NETWORK_NAME - Server's IP address or network name"
		"\n 3. SERVER_PORT - Server's port number"
		"\n 4. Use one of the following options:"
		"\n /u dir - Upload option, uploads the files in the local directory"
		"\n 5. /d - download option, Downloads all the files from the server upload directory"
		"\n 6. /s \"srch_str\" nThreads nRequests - Makes nRequests to the server for searching search string \"srch_str\" in the server upload directory with nThreads "
		"\n\nn Examples of usage:"
		"\n		e.g for upload: ClientExecutive 8181 127.0.0.1 8282 /u ../TestDir"
		"\n		e.g for download: ClientExecutive 8181 127.0.0.1 8282 /d"
		"\n		e.g for string search: ClientExecutive 8181 127.0.0.1 8282 /s \"hi\" 3 5"
		"\n \n";
		printMessage(usage);
}
 
//prints server usage
void Display::printServerUsage()
{ 
	string usage = "\n\nServer usage:"
		"\n ServerExecutive LISTENING_PORT"
		"\n 1. LISTENING_PORT - Port to listen to new incoming requests at \n\n"
		"\n e.g.: ServerExecutive 8181";
	printMessage(usage);
}


//Print useful info about the class like where are the server and client started
void Display::printInfo(string caller, string addr)
{
	string msg="";
	if (caller == "client"){
		msg = "\n\n" + std::string(25, '-') + " Client started at " + addr +" "+ std::string(30, '-') + "\n";
	}
	else{
		msg = "\n\n" + std::string(25, '-') + " Server started at port " + addr + " " +std::string(30, '-') + "\n";
	}
	printMessage(msg);
	printMessage("\nThe received files from client are stored in directory \"UploadFiles\" under the project main directory.Each received file is stored prepended with the sender's address");
	printMessage("\nThe header has a blank line at the end, which is shown by the fact that the message appearing in the header is printed in the next line, even if they are appended together without a new line in between them, while printing to output ");
	printMessage("\n\nPress Ctrl + c to terminate\n");
}

//print any message to output
void Display::printMessage(string str_msg)
{
	cout << str_msg;
}

//Destructor
Display::~Display()
{}

#ifdef DISPLAY_TEST
int main(){
	Display disp;
	disp.printClientUsage();
	disp.printServerUsage();
	disp.printInfo("client");
	disp.printInfo("server");
	disp.printMessage("\nStopping...");
}
#endif

