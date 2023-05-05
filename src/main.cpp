#include <fmt/core.h>
#include <unistd.h>
#include <thread>
#include "constants.h"
#include "data.h"
#include "theory.h"

int main() {
  nice(31);
  auto proc_count = std::thread::hardware_concurrency();

  auto data = ReadData("data.csv");
  GetModelData(data, proc_count);
  GetCordsData(data);

  for (std::size_t i = 0; i < data.t.size(); ++i) {
    fmt::print("{:.15e},{:.15e},{:.15e},{:.15e}\n", data.t[i], data.x[i],
               data.N_model[i], data.N_data[i]);
  }

  return EXIT_SUCCESS;
}
