#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// All TODOs follow // https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/process.cpp
// Also see https://github.com/rmorejon65/CppND-System-Monitor/blob/master/src/process.cpp

// TODO: Return this process's ID
int Process::Pid() { return iPid_; }

// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/process.cpp
// TODO: Return this process's CPU utilization
// Do not use this because it requires adding a second CpuUtilization function to LinuxParser: return cpuUtilization_ = LinuxParser::CpuUtilization(iPid_);
float Process::CpuUtilization() {
  long lProcJiffies = LinuxParser::ActiveJiffies(this->iPid_);
  long lTotalJiffies = LinuxParser::Jiffies();
  
  float fProcUtil = (float)lProcJiffies / (float)lTotalJiffies;
  
  this->fCpuUtilization_ = fProcUtil;
  
  return fProcUtil; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(iPid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(iPid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(iPid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(iPid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// This is supposed to return True.
bool Process::operator<(Process const& a) const { return a.fCpuUtilization_ < this->fCpuUtilization_ ; }