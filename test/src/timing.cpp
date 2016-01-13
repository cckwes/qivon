#include <chrono>

#include "timing.h"

std::chrono::system_clock::time_point g_start;

void set_start_now() {
  g_start = std::chrono::system_clock::now();
}

long long get_elapsed_time() {
  auto elapsed =  std::chrono::duration_cast<std::chrono::milliseconds>
      (std::chrono::system_clock::now() - g_start);
  return elapsed.count();
}