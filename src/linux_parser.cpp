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

// https://github.com/rmorejon65/CppND-System-Monitor/blob/master/src/linux_parser.cpp
// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/linux_parser.cpp
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return 0;
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

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string sValue, sLine ;
  long lClock; 
  vector<string> svecStatList ; 
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatFilename);
  if( filestream.is_open()){
    std::getline(filestream , sLine); 
    std::istringstream linestream(sLine); 
    while(linestream >> sValue){svecStatList.push_back(sValue);}

  }
  long int lUtime{std::stol(svecStatList[13])};
  long int lStime{std::stol(svecStatList[14])};
  long int lCutime{std::stol(svecStatList[15])};
  long int lCstime{std::stol(svecStatList[16])};

  lClock = lUtime + lStime + lCutime + lCstime ; 
  return lClock;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

// https://github.com/martycheung/CppND-System-Monitor-Project/blob/master/src/linux_parser.cpp
// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  
  vector<string> svecUtilString = LinuxParser::CpuUtilization();
  
  // add only idletime and iowait, converted from string to long
  long lIdleJiffies = stol(svecUtilString[LinuxParser::CPUStates::kIdle_]) + stol(svecUtilString[LinuxParser::CPUStates::kIOwait_]);
  
  return lIdleJiffies;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// Similar to LinuxParser::Pids()
// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string sKey, sLine ;  // Don't need string value?
  vector<string> svecJiffiesList; 
  std::ifstream filestream(kProcDirectory + kStatFilename ); 
  if (filestream.is_open())
  {
    std::getline(filestream, sLine); 
    std::istringstream linestream(sLine); 
    while(linestream >> sKey ) { 
      if (sKey != "cpu"){svecJiffiesList.push_back(sKey);}
    }
  }
  return svecJiffiesList;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
   long lTotalProcesses = 0;
  if (filestream.is_open()) {
      std::string line;
      bool bProcessNumberFound = false;
      
      while (std::getline(filestream, line) && !bProcessNumberFound) {
        std::istringstream linestream(line);
        std::string key;
        linestream >> key;
        if (key == "processes")
        {          
            linestream >> lTotalProcesses;
            bProcessNumberFound = true;
        }
      }

  }
  return lTotalProcesses; 
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
   long lRunningProcesses = 0;
  if (filestream.is_open()) {
      std::string line;
      bool bProcessNumberFound = false;
      
      while (std::getline(filestream, line) && !bProcessNumberFound) {
        std::istringstream linestream(line);
        std::string key;
        linestream >> key;
        if (key == "procs_running")
        {          
            linestream >> lRunningProcesses;
            bProcessNumberFound = true;
        }
      }

  }
  return lRunningProcesses;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string sCmd ; 
  string kPidDirectory = "/" + std::to_string(pid); 
  std::ifstream filestream (kProcDirectory + kPidDirectory + kCmdlineFilename); 
  if (filestream.is_open())
  {
    std::getline(filestream, sCmd); 

  }
  return sCmd;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string sLine, sKey; 
  long lRam ; 
  string kPidDirectory = "/" + std::to_string(pid);
  std::ifstream filestream (kProcDirectory + kPidDirectory +kStatusFilename ); 
  if (filestream.is_open())
  {
    while (std::getline(filestream, sLine))
    {
      std::istringstream linestream(sLine); 
      linestream >> sKey ; 
      if (sKey == "VmSize:"){linestream >> lRam; break ; }
    }
    
  }
  return std::to_string(lRam/1000);
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string sLine, sKey, sUid; 
  string kPidDirectory = "/" + std::to_string(pid);
  std::ifstream filestream (kProcDirectory + kPidDirectory +kStatusFilename ); 
  if (filestream.is_open()){
    std::getline(filestream, sLine); 
    std::istringstream linestream(sLine);
    while (linestream >> sKey)
    {
      if (sKey == "Uid:"){linestream >> sUid; break ; }
    }
    
  }
  return sUid ;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string sLine, sUser, sUid, sVar; 
  std::ifstream filestream (kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream, sLine)){
      std::replace(sLine.begin(), sLine.end(), ':',  ' ');
      std::istringstream linestream(sLine);
      linestream >> sUser >> sVar >> sUid ; 
      if(sUid == Uid(pid)){break; } 
    }
  }
  return sUser;
}

// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string sLine, sVar; 
  long lStartTtime = 0, uptime ; 
  vector<string> svecStatList; 
  string kPidDirectory = "/" + std::to_string(pid);
  std::ifstream filestream (kProcDirectory + kPidDirectory +kStatFilename ); 
  if (filestream.is_open()){
    std::getline(filestream, sLine); 
    std::istringstream linestream(sLine); 
    while (linestream >> sVar){svecStatList.push_back(sVar);}
    
  }
  lStartTtime = std::stol(svecStatList[21]) / sysconf(_SC_CLK_TCK);
  uptime = LinuxParser::UpTime() - lStartTtime; 
  return uptime;
}