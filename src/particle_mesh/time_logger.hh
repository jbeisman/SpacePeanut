
#pragma once

#include <chrono>
#include <map>
#include <string>

// Simple time logging class for tracking the cumulative runtime of different
// functions

class TimeLogger {
  // Start times of active functions
  std::map<std::string, std::chrono::high_resolution_clock::time_point>
      start_times_;
  // Cumulative runtimes
  std::map<std::string, double> cumulative_times_ms_;

public:
  TimeLogger() = default;
  void start(const std::string &function_name);
  void end(const std::string &function_name, bool print_now = false);
  void print_results() const;
  void reset();
};
