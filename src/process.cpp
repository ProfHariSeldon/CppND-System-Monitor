#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// All TODOs follow // https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/process.cpp
// Also see https://github.com/rmorejon65/CppND-System-Monitor/blob/master/src/process.cpp

// TODO: Return this process's ID
int Process::Pid() { return iPid_; }

// float Process::CpuUtilization code that did not work
// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/process.cpp
// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp

// float Process::CpuUtilization() code suggested by professor in thread below, but "Util::getStream(path, stream);" does not work
// https://knowledge.udacity.com/questions/225256

// https://github.com/sinamoghimi73/CppND-System-Monitor-Project/blob/master/src/process.cpp
// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return static_cast<float>(LinuxParser::ActiveJiffies(iPid_) * 100) /
         sysconf(_SC_CLK_TCK) / LinuxParser::UpTime(iPid_);
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