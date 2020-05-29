#include <string>

#include "format.h"

using std::string;

// https://github.com/Fernandoks/CppND-System-Monitor/blob/master/src/format.cpp
// ElapsedTime double-checked to work in my own Test.cpp sandbox, since how else am I supposed to test if code works if the final program is incomplete

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// DONE
string Format::ElapsedTime(long lSeconds)
{
  // Find the number of hours, throw away remainder
  int iHours = lSeconds/3600;
  // Find the remainder number of seconds, convert to minutes
  int iMinutes = (lSeconds%3600)/60;
  // Find the remainder number of seconds in the remainder number of minutes
  int iSeconds = (lSeconds%3600)%60;
  // Format result
  string sElapsedTime = std::to_string(iHours) + ":" + std::to_string(iMinutes) + ":" +std::to_string(iSeconds);
  // Return result
  return sElapsedTime;
}