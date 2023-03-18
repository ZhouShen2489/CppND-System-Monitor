#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>

#include "linux_parser.h"
//it takes the your variable and then by writing (#x) it converts the name of the variable
#define printVailableNameAndValue(x) std::cout << "The name of variable **" << (#x) << "** and the value of varibale is => " << x << "\n"

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
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

// TODO: Read and return the system memory utilization
// https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
float LinuxParser::MemoryUtilization() { 
  float mem_usage = 0.0;
  float total_mem = 0.0;
  float free_men = 0.0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          total_mem = stof(value);
        }
        else if (key == "MemFree") {
          free_men = stof(value);
        }
      }
    }
  }
  mem_usage = (total_mem - free_men) / total_mem;
  return mem_usage;
}

// TODO: Read and return the system uptime
float LinuxParser::UpTime() {
  float up_time = 0;
  string line;
  string time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stringstream(line);
    stringstream >> time;
    up_time = std::stof(time);
  }
  return up_time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long jiffies = 0;
  jiffies = ActiveJiffies() + IdleJiffies();
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {




  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  int iter = 1;
  long utime, stime, cutime, cstime;
  string item;

  if (filestream.is_open()) {
    while (filestream >> item) {
      if (iter == 14) {
        utime = stol(item);
      }
      else if (iter == 15) {
        stime = stol(item);
      }
      else if (iter == 16) {
        cutime = stol(item);
      }
      else if (iter == 17) {
        cstime = stol(item);
        break;
      }
      iter++;
    } 
  }
  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_jiffies = 0;
  vector<string> values = CpuUtilization();
  active_jiffies = stol(values[CPUStates::kUser_]) + stol(values[CPUStates::kNice_]) +
                    stol(values[CPUStates::kSystem_]) + stol(values[CPUStates::kIRQ_]) +
                    stol(values[CPUStates::kSoftIRQ_]) + stol(values[CPUStates::kSteal_]) +
                    stol(values[CPUStates::kGuest_]) + stol(values[CPUStates::kGuestNice_]); 
  return active_jiffies;
}

/*
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
*/

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle_jiffies = 0;
  vector<string> values = CpuUtilization();
  idle_jiffies = stol(values[CPUStates::kIdle_]) + stol(values[CPUStates::kIOwait_]);
  return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_uti;
  string key;
  string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    // only get the aggregate gpu
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> key) {
        if (key == "cpu") {
          while (stringstream >> key) {
            cpu_uti.push_back(key);
          }
          return cpu_uti;
        }
      }
    }
  }
  return cpu_uti;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_process = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> key >> value) {
        if (key == "processes") {
          total_process = stoi(value);
          return total_process;
        }
        
      }
    }
  }
  return total_process;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_process;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> key >> value) {
        if (key == "procs_running") {
          running_process = stoi(value);
        }
      }
    }
  }
  return running_process;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command = {};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
  }
  string shorten_command = command.substr(0, 50) + "...";
  return shorten_command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// In order to facilitate display, consider converting the memory utilization into megabytes.
string LinuxParser::Ram(int pid) {
  string mem_size={};
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> key >> value) {
        if (key == "VmRSS:") {   //use VmRSS instead of VmSize because VmRSS gives the exact physical memory being used as a part of Physical RAM
          mem_size = value;
          mem_size = mem_size.substr(0, mem_size.size() - 3); //
          return mem_size;
        }
      }
    }
  }
  return mem_size;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid = {};
  string line;
  string key;
  string uid1, uid2, uid3, uid4;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      while (stringstream >> key >> uid1 >> uid2 >> uid3 >> uid4) {
        if (key == "Uid:") {
          uid = uid1;
          return uid;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string user = {};
  string line;
  string x; // temp
  string uid;  // temp
  std::ifstream filestream(kPasswordPath);
  string find_uid = Uid(pid); // the uid we need to match
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);
      while (stringstream >> user >> x >> uid) {
        if (uid == find_uid) {
          return user;
        }
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + kStatFilename);
  int iter = 1;
  long system_up_time = LinuxParser::UpTime();
  long process_up_time = 0;
  long process_start_time = 0;
  long hertz = sysconf(_SC_CLK_TCK);
  string item;
  string line;
  while (filestream.is_open()) {
    while (filestream >> item) {
      if (iter == 22) {
        process_start_time = stol(item);
        process_up_time = system_up_time - (process_start_time / hertz);
        // printVailableNameAndValue(process_up_time);
        return process_up_time;
      }
      iter++;
    }
  }
  return process_up_time;
}

long LinuxParser::StartTime(int pid) {
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + kStatFilename);
  int iter = 1;
  long process_start_time = 0;
  string item;
  while (filestream.is_open()) {
    while (filestream >> item) {
      if (iter == 22) {
        process_start_time = stol(item);
        return process_start_time;
      }
      iter++;
    }
  }
  return process_start_time;
}