/////////////////////////////////////////////////////////////////////
//  Executive.cpp - Makes call to the FileManager, Parser and      //
//					Display	classes								   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Samsung QX411, Windows 7 Home Premium           //
//  Application:   Scope analysis, OOD Assignment 1, Spring2014    //
//  Author:		   Pallavi Iyengar                                 //
/////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include "ConfigureParser.h"
#include "Display.h"

#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[])
{
	if (argc < 2){
		std::cout << "\n Usage: [/s] [/b] path [pattern]";
		return 0;//check this
	}
	for (int i = 0; i < argc;i++){
		std::cout<<"\n"<<argv[i];
	}

	FileManager fileManager;
	fileManager.parseCmdLine(argc, argv);
	std::vector<std::string> files = fileManager.listFiles();

	for (std::vector<std::string>::iterator itr = files.begin(); itr != files.end(); ++itr){
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
	
		try
		{
		if (pParser)
		{
		if (!configure.Attach(*itr))
		{
		std::cout << "\n  could not open file " << *itr << std::endl;
		continue;
		}
		}
		else
		{
		std::cout << "\n\n  Parser not built\n\n";
		return 1;
		}
		// now that parser is built, use it
		std::cout << "\n\nfile: " << *itr << std::endl;

		while (pParser->next())
		pParser->parse();
		std::cout << "\n\n";
		std::vector<element> scopes = configure.getAnalysedScopes();
		boolean showFuncSizeandComplexityOnly = fileManager.showFuncSizeAndComplexityOnly();
		Display display(scopes, showFuncSizeandComplexityOnly);
		display.printOutput();


		/*for (auto scope : scopes){
		if (fileManager.showFuncSizeAndComplexityOnly()){
		if (scope.type == "function"){
		std::cout << "\nFunction Size = " << scope.endLine - scope.lineCount + 1 << "\n";

		std::cout << "\nFunction Complexity = " << scope.complexity << "\n";
		std::cout << "\nFunction Name = " << scope.name;

		}
		}
		else{
		std::cout << "\n\nscope Type = " << scope.type << "\nstart line = " << scope.lineCount + 1 << "\nend line = " << scope.endLine + 1;
		//<<", line count ="<<p_Repos->lineCount() - elem.lineCount + 1<<
		/*if (elem.type == "class" ||
		elem.type=="struct" ||
		elem.type=="enum" ||
		elem.type=="function"||
		elem.type=="namespace")
		{*/
		/*	std::cout << "\nScope Name = " << scope.name;
			//}
			}*/
		//}
		}
		catch (std::exception& ex)
		{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		}
	
	return 0;
}
#endif