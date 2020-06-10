#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// using Util::getStream;

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
  // https://github.com/rmorejon65/CppND-System-Monitor/blob/master/src/linux_parser.cpp
  // The entire svecUtil is linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guess >> guessnice;
  // In other words lTotalJiffies = totalUserTime (user - guess) + totalNiceTime (nice - guesnice) + totalIdleTime (idle + iowait) + totalSystem (system + irq + softirq) + totalVirtualTime (guess + guessnice)
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
      // In other words:
      // idle = params[3] + params[4];
      // active = params[0] + params[1] + params[2] + params[4] + params[6] + params[7];
      // svecJiffiesList = active/(float)(idle + active);
      if (sKey != "cpu"){svecJiffiesList.push_back(sKey);}
    }
  }
  return svecJiffiesList;
}
/*
// https://knowledge.udacity.com/questions/225256
// ERROR name (Util) followed by a '::' must be a class or namespace name
float LinuxParser::getCpuPercent(int pid)
{
	std::string path =  kProcDirectory + to_string(pid) + kStatFilename;
	std::ifstream stream;
	Util::getStream(path, stream);
	string line;
	std::getline(stream, line); // file contains only one line
	std::istringstream buffer(line);
	std::istream_iterator<string> beginning(buffer), end;
	std::vector<string> line_content(beginning, end);
	float utime = LinuxParser::UpTime(pid);
	float stime = stof(line_content[14]);
	float cutime = stof(line_content[15]);
	float cstime = stof(line_content[16]);
	float starttime = stof(line_content[21]);
	float uptime = LinuxParser::UpTime();
	float freq = sysconf(_SC_CLK_TCK);
	float total_time = utime + stime + cutime + cstime;
	float seconds = uptime - (starttime / freq);
	float result = 100.0 * ((total_time / freq) / seconds);
	return (result);
}
*/
// https://github.com/wissalsayhi/udacity-CppND---System-Monitor/blob/master/src/linux_parser.cpp
// ERROR: why is this here not in process.cpp?  process.cpp already has a float Process::CpuUtilization()
 // Cpu utilizationfor specific process
 /*
 float LinuxParser::CpuUtilProcess(int pid){  

   string line;
  vector<string> columns;
  string column;
  float util{0.0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    while(linestream.good()) {
      getline(linestream, column, ' ');
      columns.push_back(column);
    }
    //totalTime = utime + stime
    // with child processes totalTime += cutime + cstime
    int totalProcessTicks = stoi(columns[13]) + stoi(columns[14]) + stoi(columns[15]) + stoi(columns[16]);
    float totalProcessTime = totalProcessTicks / (float)sysconf(_SC_CLK_TCK);
    long totalSeconds = UpTime(pid);
    util = totalSeconds != 0 ? (totalProcessTime/(float)totalSeconds) : 0.0;
  }
  return util;

 }
 */

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
        // In other words we only care about TotalProcess no other data
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
        // In other words we only care about RunningProcesses no other data
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
    // Record the kCmdlineFilename associated with the kProcDirectory by using the key pid?
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
  std::ifstream filestream (kProcDirectory + kPidDirectory + kStatusFilename ); 
  if (filestream.is_open())
  {
    while (std::getline(filestream, sLine))
    {
      std::istringstream linestream(sLine); 
      linestream >> sKey ;
      // The data associated with the key VmSize: is the memory used by a process
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
  std::ifstream filestream (kProcDirectory + kPidDirectory + kStatusFilename ); 
  if (filestream.is_open()){
    std::getline(filestream, sLine); 
    std::istringstream linestream(sLine);
    while (linestream >> sKey)
    {
      // The data associated with the key Uid: is the user ID associated with a process
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
      // Read a line that contains a user, variable, and process ID
      linestream >> sUser >> sVar >> sUid ; 
      // If the line's process ID is the one we want, then break and return sUser
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
  long lStartTtime = 0, lUptime ; 
  vector<string> svecStatList; 
  string kPidDirectory = "/" + std::to_string(pid);
  std::ifstream filestream (kProcDirectory + kPidDirectory +kStatFilename ); 
  if (filestream.is_open()){
    std::getline(filestream, sLine); 
    std::istringstream linestream(sLine); 
    // Load up svecStatList with sVar elements
    while (linestream >> sVar){svecStatList.push_back(sVar);}
    
  }
  // We only care about the 21st sVar I'm sorry I don't know how to find out why
  lStartTtime = std::stol(svecStatList[21]) / sysconf(_SC_CLK_TCK);
  lUptime = LinuxParser::UpTime() - lStartTtime; 
  return lUptime;
}