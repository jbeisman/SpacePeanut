
#include "time_logger.hh"
#include <iostream>

void TimeLogger::start(const std::string &function_name) {
  if (start_times_.count(function_name)) {
    std::cerr << "Warning: Timer for function '" << function_name
              << "' already started. Ignoring new start.\n";
    return;
  }
  start_times_[function_name] = std::chrono::high_resolution_clock::now();
}

void TimeLogger::end(const std::string &function_name, bool print_now) {
  auto it = start_times_.find(function_name);
  if (it == start_times_.end()) {
    std::cerr << "Error: Timer for function '" << function_name
              << "' not started. Cannot stop.\n";
    return;
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end_time - it->second;
  if (print_now) {
    std::cout << function_name << ": " << duration.count() << " ms\n";
  }
  cumulative_times_ms_[function_name] += duration.count();
  start_times_.erase(it);
}

void TimeLogger::print_results() const {
  std::cout << "\n--- Function Execution Times ---\n";
  if (cumulative_times_ms_.empty()) {
    std::cout << "No functions have been timed yet.\n";
  } else {
    // print each function's cumulative time.
    for (const auto &pair : cumulative_times_ms_) {
      std::cout << "Function: " << pair.first
                << ", Cumulative Time: " << pair.second << " ms\n";
    }
  }
  std::cout << "--------------------------------\n";

  // print functions that are currently running
  if (!start_times_.empty()) {
    std::cout << "\n--- Functions Still Being Timed ---\n";
    for (const auto &pair : start_times_) {
      std::cout << "Function: " << pair.first << " (started but not ended)\n";
    }
    std::cout << "-----------------------------------\n";
  }
}

void TimeLogger::reset() {
  start_times_.clear();
  cumulative_times_ms_.clear();
  std::cout << "TimeLogger reset.\n";
}
