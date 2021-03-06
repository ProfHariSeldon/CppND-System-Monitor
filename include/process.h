#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // https://knowledge.udacity.com/questions/212681
  Process(int pid) : iPid_(pid){}

  // TODO: Declare any necessary private members
  // https://github.com/rmorejon65/CppND-System-Monitor/blob/master/include/process.h
 private:
  int iPid_ = 0;
  float fCpuUtilization_;
};

#endif