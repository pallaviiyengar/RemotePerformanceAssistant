///////////////////////////////////////////////////////////////////
// BlockingQueue.h - Thread-safe queue that blocks on empty deQ using C++ 11 threads  //
// ver 1.1                                                       //
// Language: standard C++                                        //
// Platform: Dell Dimension T7400, Windows 7, SP #1              //
// Application: Resource for DO projects                         //
// Author:      Jim Fawcett, CST 4-187, Syracuse University      //
//              jfawcett@twcny.rr.com, (315) 443-3948            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package contains a BlockingQueue<> class that is thread-safe
* and blocks a deQ'er if the queue is empty.  Blocking is accomplished
* using C++11 mutex and condition_variable.
*
* Users don't need to be aware of how this works.  They just use
* the queue without worrying about locking.
*/
/*
* Required Files:
* ---------------
* BlockingQueue.h, BlockingQueue.cpp
*
* Build Process:
* --------------
* cl /EHa /DTEST_BLOCKINGQUEUE BlockingQueue.cpp
*
* Maintenance History:
* --------------------
* ver 2.0: 24 Apr 2014
* Added method to check if queue is empty
* ver 1.0 : 24 Mar 2014
*/

#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////
// BlockingQueue<Msg>

template <typename T>
class BlockingQueue {
public:
  BlockingQueue() {}
  T deQ();
  void enQ(const T& t);
  size_t size();
  bool isEmpty();

private:
  BlockingQueue(const BlockingQueue<T>&);
  BlockingQueue<T>& operator=(const BlockingQueue<T>&);
  std::queue<T> q_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

//----< remove a message from queue >-------------------------------
template<typename T>
T BlockingQueue<T>::deQ()
{
  std::unique_lock<std::mutex> l(mtx_);
  if(q_.size() > 0)
  {
    T temp = q_.front();
    q_.pop();
    return temp;
  }
  // loop needed due to spurious returns from wait
  while (q_.size() == 0)
    cv_.wait(l, [this] () { return q_.size() > 0; });
  T temp = q_.front();
  q_.pop();
  return temp;
}

//----< add a message to queue >-------------------------------

template<typename T>
void BlockingQueue<T>::enQ(const T& t)
{
  {
    std::lock_guard<std::mutex> l(mtx_);
    q_.push(t);
  }
  cv_.notify_one();
}

//----< return number of queueud messages  >-------------------------------
template<typename T>
size_t BlockingQueue<T>::size()
{
  std::lock_guard<std::mutex> l(mtx_);
  return q_.size();
}



//----< return if empty  >-------------------------------
template<typename T>
bool BlockingQueue<T>::isEmpty()
{
	std::lock_guard<std::mutex> l(mtx_);
	return (q_.size()==0);
}

#endif

