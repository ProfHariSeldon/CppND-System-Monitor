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
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
// HELP is this the cooler alternative to: return cpuUtilization_ = LinuxParser::CpuUtilization(pid_);
float Process::CpuUtilization() { return LinuxParser::CpuUtilization(Pid()); }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// This is supposed to return True.
bool Process::operator<(Process const& a) const { return a.cpuUtilization_ < this->cpuUtilization_ ; }