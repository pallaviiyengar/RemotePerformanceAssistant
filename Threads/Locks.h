#ifndef LOCKS_H
#define LOCKS_H
///////////////////////////////////////////////////////////////
// Locks.h - Defines class for synchronised printing         //
// to the output in multi threaded applicaitons
// ver 1.2                                                   //
// Language: standard C++                                    //
// Platform: Dell Dimension T7400, Windows 7, SP #1          //
// Application: Resource for DO projects,  
// Message Passing Communication, OOD Assignment 3, Spring2014         
// Author : Pallavi Iyengar
// Source:      Jim Fawcett, CST 4-187, Syracuse University  //
//              jfawcett@twcny.rr.com, (315) 443-3948        //
///////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * ===================
 * 
 * Provides class to synchronise printing output in multi-threaded applications
 */
/*
 * Required Files:
 * ---------------
 * Locks.h, Locks.cpp
 *
 * Build Process:
 * --------------
 *- devenv MessagePassing.sln
 *
 * Maintenance History:
 * --------------------
 * ver 1.2: 10 Apr 2014
 * Used C++11 lock constructs and retained just the syncOut class
 *
 * ver 1.1 : 24 Mar 2013
 * - added sout, moved doLog here, uses latest threadBase
 * ver 1.0 : 20 Feb 2012
 * - first release
 */

#include <Windows.h>
#include <exception>
#include <iostream>
#include <mutex>
/////////////////////////////////////////////////////////////////////
// syncOut class

class syncOut
{
public:
  syncOut(std::ostream& out);
  template <typename T>
  syncOut& operator<<(const T& t);
  syncOut& operator<<(syncOut& (*op)(syncOut&));
  template <typename T>
  void operator()(const T& t);
  void flush();

private:
  std::mutex _l;
  std::ostream& _out;
};
//----< constructor >------------------------------------------------

inline syncOut::syncOut(std::ostream& out) : _out(out) {}

//----< insertion >--------------------------------------------------

template <typename T>
syncOut& syncOut::operator<<(const T& t)
{
	std::lock_guard<std::mutex> lock(_l);
  _out << t;
  _out.flush();
  return *this;
}
//----< insertion >--------------------------------------------------

inline syncOut& syncOut::operator<<(syncOut& (*op)(syncOut&))
{
  return (*op)(*this);
}
//----< functor >----------------------------------------------------

template <typename T>
void syncOut::operator()(const T& t)
{
	std::lock_guard<std::mutex> lock(_l);
	_out << t;
   _out.flush();
}
//----< flush output stream >----------------------------------------

inline void syncOut::flush() { _out.flush(); }

extern syncOut sout;

// mimics cout
#endif
