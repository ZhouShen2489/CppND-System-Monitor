#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hour = seconds / 3600;
    long minute = seconds%3600 / 60;
    long second = seconds%3600%60;
    string s_hour = hour < 10 ? "0" : "" + std::to_string(hour);
    string s_minute = minute < 10 ? "0" : "" + std::to_string(minute);
    string s_second = second < 10 ? "0" : "" + std::to_string(second);
    return s_hour + ":" + s_minute + ":" + s_second;
}