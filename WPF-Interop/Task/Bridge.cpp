///////////////////////////////////////////////////////////////
// Bridge.cpp - Supports Interop between C# and native C++   //
//  Author: PAllavi Iyengar                                                         //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////

#include "ITask.h"
#include "Task.h"
#include <string>
using namespace System;
using namespace System::Text;

ref class Bridge : public ITask
{
public:
  Bridge() : pTask(new Task(this)) {}
  ~Bridge() { delete pTask; }

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

  virtual void run_search(System::Windows::Threading::Dispatcher^ disp, String^ numThreads, String^ srchStr, String^ nReqs) override
  {
    dispatcher_ = disp;
	std::string cstrNumThreads = convert(numThreads);
	std::string csrchStr = convert(srchStr);
	std::string cnReqs = convert(nReqs);
	if (pTask!=nullptr){
		pTask->makeSearchReq(cstrNumThreads, csrchStr, cnReqs);
	}
  }

  virtual void run_connect(System::Windows::Threading::Dispatcher^ disp, String^ ip, String^ port, String^ listeningPort) override{
	  dispatcher_ = disp;
	  std::string strip = convert(ip);
	  std::string strport = convert(port);
	  std::string strcltPort = convert(listeningPort);

	  if (pTask != nullptr){
		  pTask->makeConnectReq(strip, strport, strcltPort);
	  }
  }

  virtual void run_disconnect(System::Windows::Threading::Dispatcher^ disp) override {
	  dispatcher_ = disp;
	  if (pTask != nullptr){
		  pTask->makeDisconnectReq();
	  }
  }

  virtual void doCallBack(std::string msg) override
  {
    if (callback != nullptr)
    {
	  // convert msg
		String^ mstrMsg = convert(msg);
		dispatcher_->Invoke(callback, mstrMsg);
    }
  }


private:
  Task* pTask;
  System::Windows::Threading::Dispatcher^ dispatcher_;
};

ITask^ ITask::Create()
{
  return gcnew Bridge();
}
