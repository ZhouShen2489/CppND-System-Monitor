#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#define printVailableNameAndValue(x) std::cout << "The name of variable **" << (#x) << "** and the value of varibale is => " << x << "\n"

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};


// constructor
Process::Process(int pid) : pid_(pid) {
    cpu_usage_ = CpuUtilization();
}


// TODO: Return this process's ID
int Process::Pid() {
    return pid_;
}

// TODO: Return this process's CPU utilization (process interval utilization FAIL)
// float Process::CpuUtilization() {
//     float up_time_system = LinuxParser::UpTime();  // value in second
//     long start_time_process = LinuxParser::StartTime(Pid()); // value in clock-tick
//     long current_total_time_process = LinuxParser::ActiveJiffies(Pid()); // value in clock-tick
//     long hertz = sysconf(_SC_CLK_TCK);

//     float current_seconds = up_time_system - ((float)start_time_process / hertz); // total elapsed time in seconds since the process started
//     float interval_seconds = current_seconds - previous_seconds_;
//     printVailableNameAndValue(interval_seconds);
//     long interval_total_time_process = current_total_time_process - previous_total_time_process_;
//     // printVailableNameAndValue(interval_total_time_process);

//     float interval_cpu_usage = ((float)interval_total_time_process / hertz) / interval_seconds;
//     previous_total_time_process_ = current_total_time_process;
//     previous_seconds_ = current_seconds;
//     if (interval_cpu_usage <= 0) {
//       std::cout << "Zero ";
//     }
//     else if (interval_cpu_usage >= 100) {
//       std::cout << "100 "; 
//     }
//     printVailableNameAndValue(interval_cpu_usage);

//     return interval_cpu_usage;
// }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long up_time_system = LinuxParser::UpTime();  // value in second
    long start_time_process = LinuxParser::StartTime(Pid()); // value in clock-tick
    long total_time_process = LinuxParser::ActiveJiffies(Pid()); // value in clock-tick
    long hertz = sysconf(_SC_CLK_TCK);

    float seconds = up_time_system - ((float)start_time_process / hertz); // total elapsed time in seconds since the process started
    float cpu_usage = (total_time_process / hertz) / seconds;
    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid());}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid());}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid());}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(Process::Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->cpu_usage_ > a.cpu_usage_;
}