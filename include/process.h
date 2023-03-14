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
  Process(int pid);

  // TODO: Declare any necessary private members
 private:
   int pid_;
   float cpu_usage_;
   long previous_total_time_process_ = 0;  // the previous total time of process (previous active jiffiles of pid)
   float previous_seconds_ = 0.0; // the previous seconds (previous total elapsed time in seconds since the process started)

};

#endif