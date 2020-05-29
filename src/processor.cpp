#include "processor.h"
#include "linux_parser.h"

using std::stof;

// https://github.com/rmorejon65/CppND-System-Monitor/blob/master/src/processor.cpp

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return std::stof(LinuxParser::CpuUtilization()[0]); }