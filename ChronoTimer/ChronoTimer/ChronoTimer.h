///////////////////////////////////////////////////////////////
// ChronoTimer.h - uses Chrono::high_resolution_clock        //
//                                                           //
// Platfrom:                                                 //
// Application:  Project #2 help,                            //
//               CSE687 Object Oriented Design, Spring 2014  //
//Author:		 Pallavi                                      //
// Source:       Jim Fawcett, CST 4-187, Syracuse University //
///////////////////////////////////////////////////////////////
/*
  Package Operations:
  -------------------
  Class Timer provides stop watch functionality with start(), stop(),
  and elapsedTime<T>(), where T can be any one of the following:
  std::chrono::minutes
  std::chrono::seconds
  std::chrono::milliseconds
  std::chrono::microseconds
  std::chrono::nanoseconds
  The return value of elapsedTime<T>() is an integer value representing
  the elapsed time in the specified precision.

  Required files:
  ---------------
  ChronoTimer.h, ChronoTimer.cpp

  Build command:
  --------------
  devenv ChronoTimer.sln /rebuild debug

  Maintanence History:
  --------------------
  ver 1.2: 26 apr 2014
  Get current time since epoch in miliseconds. This is for timestamping processing requests.

  ver 1.1: 21 Mar 12
  Changed return type of elapsedTime() from long to long long

  ver 1.0 : 23 Jan 14
  - first release
*/
#ifndef CHRONO_H
#define CHRONO_H
#include <chrono>
#include <string>
#include <time.h>
#include <ctime>
#include <sstream> 
#include<iomanip>
class Timer
{
public:
  Timer() {}
  void start();
  void stop();
  template <typename T>  // T is time unit, e.g., std::chrono::microsecond
  long long elapsedTime();
  long long  getCurrentTime();

  //template <typename T>
  //long long makeTimePoint(std::string time);
  //std::string getCurrentTimeInString();
  //std::string asString(const std::chrono::system_clock::time_point& tp);

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> tstart;
  std::chrono::time_point<std::chrono::high_resolution_clock> tstop;
};

//start the timer
inline void Timer::start() 
{
  tstart = std::chrono::high_resolution_clock::now();
}

//Gets current time since epoch in miliseconds
inline long long Timer::getCurrentTime(){
 long long milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return milliseconds_since_epoch;
}

//stop timer
inline void Timer::stop()
{
  tstop = std::chrono::high_resolution_clock::now();
}

//----< returns number of time units defined by T >------------------
template <typename T>
long long Timer::elapsedTime()
{
  return std::chrono::duration_cast<T>(tstop - tstart).count();
}

//inline std::string Timer::getCurrentTimeInString(){
//return (asString(std::chrono::high_resolution_clock::now()));
//}
//
////reference: http://www.informit.com/articles/article.aspx?p=1881386&seqNum=2
//inline std::string Timer::asString(const std::chrono::system_clock::time_point& tp)
//{
//	// convert to system time:
//	std::time_t t = std::chrono::system_clock::to_time_t(tp);
//	std::string ts = ctime(&t);   // convert to calendar time
//	ts.resize(ts.size() - 1);       // skip trailing newline
//	return ts;
//}
//inline std::chrono::system_clock::time_point makeTimePoint(const std::string timeStr){
//std::tm tm;
//std::stringstream ss(timeStr);
//ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S");
//auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
//  return std::chrono::duration_cast<T>(tstop - tstart).count();
//
//return tp;
//}
//	
//template <typename T>
//inline long long Timer::makeTimePoint(const std::string timeStr){
//	std::tm tm;
//	std::stringstream ss(timeStr);
//	ss >> std::get_time(&tm, "%a %b %d %Y %H:%M:%S");
//	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
//	return std::chrono::duration_cast<T>(tstop - tstart).count();
//}
//
#endif