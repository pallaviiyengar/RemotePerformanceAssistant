/////////////////////////////////////////////////////////////////
// Display.h - Display class which prints useful general info about the
// calling executive class
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:	   Samsung QX411, Windows 7 Home Premium           //
//  Application:   Message passing, OOD Assignment 3, Spring2014    //
//  Author:		   Pallavi Iyengar				 
/////////////////////////////////////////////////////////////////
/*
* Module operation:
* prints useful general info about the calling executive class
*
* Public interface:
*	Display disp;
	disp.printClientUsage();
	disp.printServerUsage();
	disp.printInfo("client");
	disp.printInfo("client", "localhost:8080");
	disp.printInfo("server");
	disp.printInfo("server", "localhost:8181");

	disp.printMessage("\nStopping...");
*
*Build Process:
*- devenv MessagePassing.sln
*
* Required files:
* - Locks.h, Locks.cpp
*  Display.cpp
*
* Maintanence History:
* ver 1.0 - 10 Apr 2014
* - first release
*/
#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
#include "../Threads/Locks.h"
using namespace std;
class Display
{
public:
	Display();
	~Display();
	void printServerUsage();
	void printClientUsage();
	void printInfo(string caller);
	void printInfo(string caller, string addr);

private:
	void printMessage(string msg);
};
#endif