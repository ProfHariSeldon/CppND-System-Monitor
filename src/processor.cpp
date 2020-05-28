#include "processor.h"

// https://github.com/rmorejon65/CppND-System-Monitor/blob/master/src/processor.cpp

// TODO: Return the aggregate CPU utilization
// HELP is [0] really required?
float Processor::Utilization() { return std::stof(LinuxParser::CpuUtilization()[0]); }