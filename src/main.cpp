#include <fmt/color.h>
#include <thread>
#include "constants.h"
#include "theory.h"

int main() {
  std::size_t proc_count = std::thread::hardware_concurrency();
  if (proc_count > 16) {
    proc_count = 16;
  }

  for (const auto& [x, y] : GetData(0, tN, proc_count)) {
    fmt::print("{:.15e} {:.15e}\n", x, y);
  }

  return EXIT_SUCCESS;
}
