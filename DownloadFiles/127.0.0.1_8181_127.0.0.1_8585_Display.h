#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////
//  FileManager.h - Parses command line and lists files in a       //
//  specified path, according to a specified pattern,recursively   //
//  or not depending on the option provided						   //
//																   // 
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that provides services necessary to parse
a command line and list files to be scope analysed.
It identifies a set of files by parsing the following command line  arguments:
1. a Path
2. One or more file pattern
3. /s, if present, the entire directory rooted at the path is searched
4. /b, if present, shows only function size and complexity of analysed functions

Public Interface:
=================
FileManager fileManager;										//Get FileManager Instance
fileManager.parseCommandLine(argc, argv);						//Parse command line arguments and list files
std::vector<std::string> files = fileManager.getListOfFiles();	//Retrieve the files listed according to the command line arguments

Build Process:
==============
Required files
- FileSystem.h, FileSystem.cpp
Build commands (either one)
- devenv FileManager.sln

Maintenance History:
====================
ver 1.0 : 11 Feb 2014
- first release

*/

#include<vector>
#include "ConfigureParser.h"
class Display{
private:
	std::vector<element> _scopes;
	bool _showFuncSizeAndComplexityOnly;
public:
	Display(std::vector<element> scopes, bool showFuncSizeAndComplexityOnly);
	void printOutput();
};
#endif