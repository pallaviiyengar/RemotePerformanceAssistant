#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09    
//  Author:		   Pallavi Iyengar
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

struct element
{
  std::string type;
  std::string name;
  size_t lineCount;
  size_t endLine;
  int complexity;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")"; 
    return temp.str();
  }
};
///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
  ScopeStack<element> stack;
  Toker* p_Toker;
  int curFuncComplexity;
  //element* curFunc;
  std::vector<element> analysedScopes;

public:
  Repository(Toker* pToker) :curFuncComplexity(0)
  {
    p_Toker = pToker;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->lines());
  }
  int getCurFuncComplexity(){
	return curFuncComplexity;
  }
  void setCurFuncComplexity(int complexity){
	curFuncComplexity = complexity;
  }
  /*void storeScope(element toStoreScope){
	  scopes.push_back(toStoreScope);
  }*/
  std::vector<element>& getanalysedScopes(){
	return analysedScopes;
  }

};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
	elem.endLine = 0;
	elem.complexity = 0;
    p_Repos->scopeStack().push(elem);

	//Modified Pallavi
//	std::cout << "\nPushing Anonymous element\n";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    if(p_Repos->scopeStack().size() == 0)
      return;
    element elem = p_Repos->scopeStack().pop();

	//Storing the analysed scope
	elem.endLine = p_Repos->lineCount();
	if (elem.type == "function"){
		elem.complexity = p_Repos->getCurFuncComplexity();
	}
	(p_Repos->getanalysedScopes()).push_back(elem);
	
	//Modified Pallavi
	//std::cout << "\nPopping element:\n";
    //pif(elem.type == "function")
    //p{
    //std::cout << "\nHandlePop";
    //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
	//    std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
	//Modified Pallavi
	
	/*std::cout << "\n\nElement Type = " << elem.type << "\nstart line = " << elem.lineCount+1 << "\nend line = " << p_Repos->lineCount()+1;
	//<<", line count ="<<p_Repos->lineCount() - elem.lineCount + 1<<
	/*if (elem.type == "class" || 
		elem.type=="struct" || 
		elem.type=="enum" || 
		elem.type=="function"||
		elem.type=="namespace")
	{*/
	//	std::cout << "\nElement Name = " << elem.name;
	//}
	/*if (elem.type=="function"){
		std::cout << "\nFunction Complexity = " << p_Repos->getCurFuncComplexity()<<"\n";
	}*/
	//p}
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--PreprocStatement rule";
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        //std::cout << "\n--FunctionDefinition rule";
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
	elem.endLine = 0;
	elem.complexity = 0;
    p_Repos->scopeStack().push(elem);
	
	p_Repos->setCurFuncComplexity(0);
	
	//p_Repos->setCurFunc(&elem);
	//p_Repos->setComplexity(1);

	//element* tmp = p_Repos->getCurFunc();

	//element tmpPopped = p_Repos->scopeStack().pop();



	//Modified Pallavi
	//std::cout << "Pushing Function Definition: "<<name<<"\n";

  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect Keywords like for, while, switch, if, catch

class SpecialKeyword : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				//std::cout << "\n--FunctionDefinition rule";
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push special keyword onto ScopeStack

class PushSpecialKeyword : public IAction
{
	Repository* p_Repos;
public:
	PushSpecialKeyword(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();
		// push  scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		element elem;
		elem.type = "special key word";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		elem.endLine = 0;
		elem.complexity = 0;
		p_Repos->scopeStack().push(elem);
		
		int complexity = p_Repos->getCurFuncComplexity() + 1;
		p_Repos->setCurFuncComplexity(complexity);
		
		//std::cout << "Pushing Special Key Word: "<<name<<"\n";

	}
};

///////////////////////////////////////////////////////////////
// rule to detect class, struct, enum
class TypeKeyword : public IRule
{
public:
	/*bool isTypeKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= {"class", "struct", "enum" };
		for (int i = 0; i<3; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}*/
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			unsigned int posCurlyBrace = tc.length() - 1;
			if (tc.find("class") < posCurlyBrace
				|| tc.find("struct") < posCurlyBrace
				|| tc.find("enum") < posCurlyBrace
				|| tc.find("namespace") < posCurlyBrace

				){
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push keywords like class, struct, enum onto ScopeStack

class PushTypeKeyword : public IAction
{
	Repository* p_Repos;
public:
	PushTypeKeyword(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		const static std::string type_keys[] = {"class", "struct", "enum", "namespace"};
		size_t len = -1;
		for (int i = 0; i < 4; i++){
			size_t tmpLen = (*pTc).find(type_keys[i]);
			if (tmpLen < (*pTc).length()){
				len = tmpLen;
				break;
			}
		}

		// push function scope
		/*size_t len = (*pTc).find("class");
		if (len >= (*pTc).length()){
			len = (*pTc).find("struct");
			if (len >= (*pTc).length()){
				len = (*pTc).find("enum");
			}
		}*/

		std::string name = "anonymous";

		//Checking if not anonymous type
		if ((len!=-1) && 
			((len + 1) < (*pTc).length()) && 
			((*pTc)[len + 1] != "{")){
			name = (*pTc)[len + 1];
		}

		element elem;
		elem.type = "Type key word";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		elem.endLine = 0;
		elem.complexity = 0;
		p_Repos->scopeStack().push(elem);

		//std::cout << "Pushing Type Key Word : " << name << "\n";


	}
};



#endif
