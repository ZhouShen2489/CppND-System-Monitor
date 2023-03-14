#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float previous_active_jiffies_ = 0.0;
  float previous_total_jiffies_ = 0.0;
};

#endif