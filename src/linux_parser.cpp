#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/linux_parser.cpp
// Similar to string LinuxParser::Kernel()
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float fTotalMem, fFreeMem, fMemUtil; //initialise all three as floats since the output of the calculation will be float
  string sLine1, sLine2;
  string sLabel1, sLabel2;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Call getline to get first line - total memory
    std::getline(stream, sLine1);
    std::istringstream linestream1(sLine1);
    linestream1 >> sLabel1 >> fTotalMem;
    
    // Call getline again to get second line - free memory
    std::getline(stream, sLine2);
    std::istringstream linestream2(sLine2);
    linestream2 >> sLabel2 >> fFreeMem;
  }
  
  // Memory Utilisation calculation
  fMemUtil = (fTotalMem - fFreeMem) / fTotalMem;
  
  return fMemUtil;
}

// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/linux_parser.cpp
// Similar to string LinuxParser::Kernel()
// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long lUptime;
  string sLine;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if (stream.is_open()) {
    // Call getline to get first line and just parse first token (uptime)
    std::getline(stream, sLine);
    std::istringstream linestream(sLine);
    linestream >> lUptime;
  }
  
  return lUptime;
}

// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/linux_parser.cpp
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long lTotalJiffies=0;
  vector<string> svecUtil = LinuxParser::CpuUtilization();
  
  // loop through and sum up all except for kGuest_ and kGuestNice_, since they are already accounted for in usertime and nicetime (according to htop source code)
  for (unsigned int i = 0; i < svecUtil.size(); i++) {
    if(i != LinuxParser::CPUStates::kGuest_ && i != LinuxParser::CPUStates::kGuestNice_) {
      // stol converts string to long int
      lTotalJiffies += stol(svecUtil[i]);
    }
  }
  
  return lTotalJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid/*[[maybe_unused]]*/) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Similar to LinuxParser::Pids()
// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid/*[[maybe_unused]]*/) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid/*[[maybe_unused]]*/) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid/*[[maybe_unused]]*/) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid/*[[maybe_unused]]*/) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid/*[[maybe_unused]]*/) { return 0; }