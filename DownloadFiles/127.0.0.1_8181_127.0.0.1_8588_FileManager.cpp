/////////////////////////////////////////////////////////////////////
//  FileManager.cpp - Parses command line and lists files		   // 
//					  accordingly                                  //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include "FileManager.h"
#include <vector>
using namespace FileSystem;

FileManager::FileManager() : bShowFuncSizeComplexity(false), bListRecursively(false){}

//----< parses command line arguments>--------------------------------
void FileManager::parseCmdLine(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++){
		std::string tmpArg = argv[i];
		std::cout << "\n args:"<<tmpArg;
		if (tmpArg == "/s"){
			bListRecursively = true;
		}
		else if (tmpArg == "/b"){
			bShowFuncSizeComplexity = true;
		}
		else if (path.empty()){
			path = argv[i];
		}
		else{
			searchPatterns.push_back(tmpArg);
		}
	}

	//If the user has entered no pattern, then consider all files in the directory
	/*if (searchPattern.empty()){
		searchPattern.push_back("*.cpp");
	}*/
}

//----< Lists the file recursively, if listRecursively is true >--------------------------------
void FileManager::searchFiles(std::string path, std::string pattern, boolean listRecursively)
{
	std::vector<std::string> tmpFiles = Directory::getFiles(path, pattern);
	for (std::vector<std::string>::iterator itr = tmpFiles.begin(); itr != tmpFiles.end(); ++itr){
		files.push_back(path + "\\" + *itr);
	}
	if (listRecursively){
		std::vector<std::string> dirs = Directory::getDirectories(path);
		for (std::vector<std::string>::iterator itr = dirs.begin(); itr != dirs.end(); ++itr){
			if (*itr == "." || *itr == ".."){
				continue;
			}
			searchFiles(path + "\\" + *itr, pattern, listRecursively);
		}
	}

}

// searches and returns list of files found
std::vector<std::string> FileManager::listFiles(){
	for (auto pattern : searchPatterns){
		searchFiles(path, pattern, bListRecursively);
	}
	return files;
}

boolean FileManager::showFuncSizeAndComplexityOnly(){
	return bShowFuncSizeComplexity;
}

//test Stub
#ifdef TEST_FILE_MANAGER
int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++){
		std::cout << "\n arg:" << argv[i];
	}
	if (argc < 2){
		std::cout << "\n Usage: [/s] [/b] path [pattern]";
		return 1;//check this
	}
	else{
		boolean pathExists = false;
		for (int i = 1; i < argc; i++){
			if (argv[i][0] != '/'){
				pathExists = true;
				break;
			}
		}
		if (!pathExists){
			std::cout << "\n Please enter a path";
			return 1;//check this
		}
	}
	FileManager fileManager;
	fileManager.parseCmdLine(argc, argv);
	std::vector<std::string> files = fileManager.listFiles();
	std::cout<<"\nFound files:";
	for(auto file : files){
		std::cout<<"\n "<<file;
	}

	boolean isDisplayLess = fileManager.showFuncSizeAndComplexityOnly();
	std::cout << "\nDisplay only file size and complexity: "<<isDisplayLess;

	return 0; 
}
#endif
