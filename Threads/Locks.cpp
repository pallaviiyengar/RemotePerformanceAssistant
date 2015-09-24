///////////////////////////////////////////////////////////////
// Locks.h - Define Lock classes based on:                   //
//             CriticalSection, SlimReaderWriter, Mutex      // 
// ver 1.1                                                   //
// Language: standard C++                                    //
// Platform: Dell Dimension T7400, Windows 7, SP #1          //
// Application: Resource for DO projects           
// Author: Pallavi Iyengar
// Source:      Jim Fawcett, CST 4-187, Syracuse University  //
//              jfawcett@twcny.rr.com, (315) 443-3948        //
///////////////////////////////////////////////////////////////

#include "Locks.h"
#include <iostream>

//----< globally available output stream >---------------------------

syncOut sout(std::cout);  // sout object
                          // mimics cout

#ifdef TEST_LOCKS

#include "Threads.h"
#include "../NewThreadsAndLocks/HiResTimer.h"

///////////////////////////////////////////////////////////////
// test derived class


void run()
{
  for(size_t i=0; i<25; ++i)
  {
	sout<< "\nmsg from child";
  }
  doLog("child exiting");
}
//----< test stub >--------------------------------------------


int main()
{
  std::cout << "\n  Demonstrating synchronised output";
  std::cout << "\n ===============================\n";
  std::thread t1(run);

    for(size_t i=0; i<25; ++i)
    {
		sout<<"\nmsg from parent";
    }
  
	t1.join();

  sout << "\n  parent exiting\n\n";
}

#endif
