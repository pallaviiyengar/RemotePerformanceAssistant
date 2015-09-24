/////////////////////////////////////////////////////////////////////
// CppCLI-client.cpp - C++\CLI using native DLL with C++11 thread  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014       //
/////////////////////////////////////////////////////////////////////
/*
 *  C++\CLI code will not compile if it attempts to use a C++11 thread.
 *  You need to isolate the threaded code behind an interface and
 *  object factory in native C++ (don't compile with /CLR option).  
 *  
 *  Then package that component as a DLL and have the C++\CLI code make
 *  a reference to the DLL.  Now it will compile and run.
 *
 *  This code shows you how to do that.  Compile the C++\CLI code with
 *  the /clr option set in the General Project Properties.  Make sure
 *  that it has a reference to the native DLL and includes the DLL Lib file.
 *
 *  Use the dll protocol shown in Comm.cpp and compile without /CLR option.
 *  Now you can build and run the project.
 */
#include "../IComm/IComm.h"
#include <string>

using namespace System;
using namespace System::Text;

String^ convert(const std::string& s)
{
  StringBuilder^ sb = gcnew StringBuilder();
  for (char ch : s)
    sb->Append(static_cast<wchar_t>(ch));
  return sb->ToString();
}

std::string convert(String^ s)
{
  std::string temp;
  for (int i = 0; i < s->Length; ++i)
    temp += static_cast<char>(s[i]);
  return temp;
}

int main(array<System::String ^> ^args)
{
  Console::Write("\n  C++\\CLI client using native C++ DLL with C++11 thread");
  Console::Write("\n =======================================================\n");

  // using mock channel that pretends to send and receive messages

  string srcAddr = "127.0.0.1", srcPort = "8484", targetAddr = "127.0.0.1", targetPort = "8181";
	  //, dir = argv[4];

  IComm* pIComm = IComm::Create();
  pIComm->setSrcAddr(srcAddr, srcPort);
  pIComm->start();
  pIComm->postEchoMessage(targetAddr, targetPort, "hi");
  string echo = pIComm->getEchoMessage();
 // sout << "\n Received message is :" << echo;
  Console::Write("\n  receiving {0}", convert(echo));
  string dir = "D:\\APR12_SUB\\SocketDemo2 - sender\\ReceivedFiles-copy";

 // pIComm->upload(targetAddr, targetPort, dir);

  //pIComm->download(targetAddr, targetPort, "127.0.0.1_8484_zzzzimage.jpg");
  //dir = "D:\\APR12_SUB\\SocketDemo2 - sender\\ReceivedFiles";
  pIComm->searchString(targetAddr, targetPort, dir, "wtf", "5");

  pIComm->wait(); 

  //IComm* pIComm = IComm::Create();
  //String^ msg = L"first message";
  //Console::Write("\n  sending {0}", msg);
  //pIComm->PostMessage(convert(msg));  
  //msg = "second message";
  //Console::Write("\n  sending {0}", msg);
  //pIComm->PostMessage(convert(msg));
  //msg = "third message";
  //Console::Write("\n  sending {0}", msg);
  //pIComm->PostMessage(convert(msg));
  //msg = "stop";
  //Console::Write("\n  sending {0}", msg);
  //pIComm->PostMessage(convert(msg));

  //Console::Write("\n  receiving {0}", convert(pIComm->GetMessage()));
  //Console::Write("\n  receiving {0}", convert(pIComm->GetMessage()));
  //Console::Write("\n  receiving {0}", convert(pIComm->GetMessage()));
  //Console::Write("\n  receiving {0}", convert(pIComm->GetMessage()));
  Console::Write("\n\n");
  return 0;
}
