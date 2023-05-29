#include <fmt/core.h>
#include <unistd.h>
#include <cstdio>
#include <thread>

#include "constants.h"
#include "data.h"
#include "data_stat.h"
#include "gauss_newton.h"
#include "theory.h"

int main() {
  nice(31);
  auto proc_count = std::thread::hardware_concurrency();

  auto data = ReadData("data.csv");

  DataSet params(186.388e-3, 2092.3, 4.23e11, 3.214, 1e20);
  GetModelData(data, params, 1);
  params.err = ComputeSqErr(data);

  auto run_data = ApplyMonteKarlo(data, params, 3000, proc_count);
  params = run_data.at(0);

  GetModelData(data, params, 1);
  GetCordsData(data, params);

  for (std::size_t i = 0; i < data.t.size(); ++i) {
    fmt::print("{:.15e},{:.15e},{:.15e},{:.15e}\n", data.t[i], data.x[i],
               data.N_model[i], data.N_data[i]);
  }

  auto range_r0 = GetValueRange(run_data);
  fmt::print(stderr, "R0 range: [ {:.5e}; {:.5e} ]\n", range_r0[0],
             range_r0[1]);

  fmt::print(stderr,
             "Error: {:.5e}\nt0 = {:.5e}\nL0 = {:.5e}\nB0 = {:.5e}\nR0 = "
             "{:.5e}\n",
             params.err, params.t0_, params.l0_, params.b0_, params.r0_);

  return EXIT_SUCCESS;
}
