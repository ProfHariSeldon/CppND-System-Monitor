#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/system.cpp

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// https://knowledge.udacity.com/questions/212681
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear(); // make sure that the vector is clear 
  auto pids=LinuxParser::Pids(); // get a vector of all pids
  for(int item:pids){ // iterate over each pid
    // make a new object from Process class using its PID and then add the object to the processes_ vector
    processes_.push_back(Process(item)); 
  }  
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }