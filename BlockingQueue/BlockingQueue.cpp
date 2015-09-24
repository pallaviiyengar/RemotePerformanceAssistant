///////////////////////////////////////////////////////////////
// Cpp11-BlockingQueue.cpp - Thread-safe Blocking Queue  using c++11
// constructs
//																	//
// Author: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>
#include "BlockingQueue.h"
std::mutex ioLock;

//test class for blocking queue
void test(BlockingQueue<std::string>* pQ)
{
	std::string msg;
	do
	{
		msg = pQ->deQ();
		{
			std::lock_guard<std::mutex> l(ioLock);
			std::cout << "\n  thread deQed " << msg.c_str();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	} while (msg != "quit");
}
#ifdef TEST_BLOCKING_QUEUE
int main()
{
  std::cout << "\n  Demonstrating C++11 Blocking Queue";
  std::cout << "\n ====================================";

  BlockingQueue<std::string> q;
  std::thread t(test, &q);

  for(int i=0; i<25; ++i)
  {
    std::ostringstream temp;
    temp << i;
    std::string msg = std::string("msg#") + temp.str();
    {
      std::lock_guard<std::mutex> l(ioLock);
      std::cout << "\n   main enQing " << msg.c_str();
    }
    q.enQ(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
  if (q.isEmpty()){
	  std::cout << "\n  Queue is empty " << msg.c_str();
  }
  t.join();

}

#endif