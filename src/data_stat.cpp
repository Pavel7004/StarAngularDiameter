#include "data_stat.h"

#include <absl/random/bit_gen_ref.h>
#include <absl/random/random.h>
#include <fmt/core.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <future>
#include <vector>

#include "constants.h"
#include "datavec.h"
#include "theory.h"

namespace {
struct DataSet {
  explicit DataSet(double t0, double L0, double B0, double m, double err)
      : t0(t0), L0(L0), B0(B0), m(m), err(err) {}

  ~DataSet() = default;

  DataSet(const DataSet&) = default;
  DataSet(DataSet&&) = default;
  DataSet& operator=(const DataSet&) = default;
  DataSet& operator=(DataSet&&) = default;

  double t0;
  double L0;
  double B0;
  double m;

  double err;
};

DataSet generateInputData(const DataSet& set, absl::BitGenRef gen) {
  constexpr double kRandRange = 0.2;

  const double t0_range = set.t0 * kRandRange;
  const double l0_range = set.L0 * kRandRange;
  const double b0_range = set.B0 * kRandRange;
  const double m_range = set.m * kRandRange;

  return DataSet(set.t0 + absl::Uniform(gen, -t0_range, t0_range),
                 set.L0 + absl::Uniform(gen, -l0_range, l0_range),
                 set.B0 + absl::Uniform(gen, -b0_range, b0_range),
                 set.m + absl::Uniform(gen, -m_range, m_range), 1e20);
}

DataSet monteCarloWorker(DataArray& data, const DataSet in_data,
                         std::size_t passes, const std::size_t& threads) {
  absl::BitGen gen;

  DataSet best_sample(0, 0, 0, 0, 1e20);
  DataSet sample(0, 0, 0, 0, 0);
  while (passes > 0) {
    sample = generateInputData(in_data, gen);

    t0 = sample.t0;
    L0 = sample.L0;
    B0 = sample.B0;
    m = sample.m;

    GetModelData(data, threads);
    sample.err = ComputeSqErr(data);
    fmt::print(stderr, "Got error: {:.15e}\n", sample.err);

    if (sample.err < best_sample.err) {
      best_sample = std::move(sample);
    }

    --passes;
  }

  return best_sample;
}

}  // namespace

double ComputeSqErr(const DataArray& data) {
  return std::sqrt(
      (std::abs(data.N_model * data.N_model - data.N_data * data.N_data))
          .sum() /
      static_cast<double>(data.N_model.size()));
}

void ApplyMonteKarlo(DataArray& data, std::size_t passes, std::size_t threads) {
  const std::size_t calc_threads = 10;
  const std::size_t rand_threads = threads / calc_threads - 1;

  auto err = ComputeSqErr(data);

  DataSet best(t0, L0, B0, m, err);
  passes = (rand_threads == 0) ? passes : passes / rand_threads;

  std::vector<std::future<DataSet>> results(rand_threads);
  for (auto& res : results) {
    res = std::async(std::launch::async, monteCarloWorker, std::ref(data), best,
                     passes, std::cref(calc_threads));
  }

  auto in_data = monteCarloWorker(data, best, passes, calc_threads);
  if (in_data.err < best.err) {
    best = std::move(in_data);
  }

  for (auto& res : results) {
    in_data = res.get();

    if (in_data.err < best.err) {
      best = std::move(in_data);
    }
  }

  t0 = best.t0;
  L0 = best.L0;
  B0 = best.B0;
  m = best.m;

  GetModelData(data, threads);
}
