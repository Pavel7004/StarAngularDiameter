#include <fmt/core.h>
#include <thread>
#include "constants.h"
#include "data.h"
#include "theory.h"

int main() {
  auto proc_count = std::thread::hardware_concurrency();

  auto data = ReadData("data.csv");
  auto model = GetData(0, tN, proc_count);

  for (const auto& [x, y] : model) {
    fmt::print("{:.15e} {:.15e}\n", x, y);
  }

  return EXIT_SUCCESS;
}
