#include "processor.h"
#include "linux_parser.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long current_active_jiffies = LinuxParser::ActiveJiffies();
    long current_total_jiffies = LinuxParser::Jiffies();
    long interval_active_jiffies = current_active_jiffies - previous_active_jiffies_;
    long interval_total_jiffies = current_total_jiffies - previous_total_jiffies_;
    float interval_utilization = (float)interval_active_jiffies / interval_total_jiffies;
    previous_active_jiffies_ = current_active_jiffies;
    previous_total_jiffies_ = current_total_jiffies;
    return interval_utilization;
}