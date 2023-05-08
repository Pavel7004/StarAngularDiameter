#include <fmt/core.h>
#include <unistd.h>
#include <cstdio>
#include <thread>
#include "constants.h"
#include "data.h"
#include "data_stat.h"
#include "theory.h"

int main() {
  nice(31);
  auto proc_count = std::thread::hardware_concurrency();

  auto data = ReadData("data.csv");
  GetCordsData(data);
  GetModelData(data, proc_count);
  ApplyMonteKarlo(data, 30, proc_count / 2);
  auto err = ComputeSqErr(data);

  for (std::size_t i = 0; i < data.t.size(); ++i) {
    fmt::print("{:.15e},{:.15e},{:.15e},{:.15e}\n", data.t[i], data.x[i],
               data.N_model[i], data.N_data[i]);
  }
  fmt::print(
      stderr,
      "Error: {:.15e}\nt0 = {:.15e}\nL0 = {:.15e}\nB0 = {:.15e}\nm = {:.15e}\n",
      err, t0, L0, B0, m);

  return EXIT_SUCCESS;
}
