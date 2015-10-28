#ifndef ITASK_H
#define ITASK_H
///////////////////////////////////////////////////////////////
// ITask.cpp - Abstract Interface for long task              //
// Author: Pallavi Iyengar                                                          //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////
#include <string>
using namespace System;
using namespace System::Text;

public ref class ITask abstract
{
public:
  delegate void EventCallBack(String^);  // callback status to MainWindow
  EventCallBack^ callback = nullptr;
  virtual void doCallBack(std::string i) = 0;

  virtual void run_search(System::Windows::Threading::Dispatcher^ disp, String^ numThreads, String^ srchStr, String^ nReqs) = 0;
  virtual void run_connect(System::Windows::Threading::Dispatcher^ disp, String^ ip, String^ port, String^ listeningPort) = 0;
  virtual void run_disconnect(System::Windows::Threading::Dispatcher^ disp) = 0;

  static ITask^ Create();  // factory function
};

#endif
