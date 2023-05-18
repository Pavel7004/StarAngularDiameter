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
  explicit DataSet(double t0, double L0, double B0, double R0, double err)
      : t0_(t0), l0_(L0), b0_(B0), r0_(R0), err(err) {}

  ~DataSet() = default;

  DataSet(const DataSet&) = default;
  DataSet(DataSet&&) = default;
  DataSet& operator=(const DataSet&) = default;
  DataSet& operator=(DataSet&&) = default;

  double t0_;
  double l0_;
  double b0_;
  double r0_;

  double err;

  inline void SetGlobalParams() const noexcept {
    t0 = t0_;
    L0 = l0_;
    B0 = b0_;
    R0 = r0_;
  }
};

DataSet generateInputData(const DataSet& set, absl::BitGenRef gen) {
  constexpr double kRandRange = 0.2;

  const double t0_range = set.t0_ * kRandRange;
  const double l0_range = set.l0_ * kRandRange;
  const double b0_range = set.b0_ * kRandRange;
  const double r0_range = set.r0_ * kRandRange;

  return DataSet(set.t0_ + absl::Uniform(gen, -t0_range, t0_range),
                 set.l0_ + absl::Uniform(gen, -l0_range, l0_range),
                 set.b0_ + absl::Uniform(gen, -b0_range, b0_range),
                 set.r0_ + absl::Uniform(gen, -r0_range, r0_range), 1e20);
}

DataSet monteCarloWorker(DataArray data, const DataSet in_data,
                         std::size_t passes, const std::size_t& threads) {
  absl::BitGen gen;

  DataSet best_sample(0, 0, 0, 0, 1e20);
  DataSet sample(0, 0, 0, 0, 0);
  while (passes > 0) {
    sample = generateInputData(in_data, gen);

    sample.SetGlobalParams();
    GetModelData(data, threads);
    sample.err = ComputeSqErr(data);

    // fmt::print(stderr, "Got error: {:.10e}\n", sample.err);

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
  // NOTE: number of threads to create beside main one
  const std::size_t rand_threads = 3;
  const std::size_t calc_threads = threads / (rand_threads + 1);

  DataSet best(t0, L0, B0, R0, ComputeSqErr(data));
  passes = passes / (rand_threads + 1);

  std::vector<std::future<DataSet>> results(rand_threads);
  for (auto& res : results) {
    res = std::async(std::launch::async, monteCarloWorker, data, best, passes,
                     std::cref(calc_threads));
  }

  auto res_sample = monteCarloWorker(data, best, passes, calc_threads);
  if (res_sample.err < best.err) {
    best = std::move(res_sample);
  }

  for (auto& res : results) {
    res_sample = res.get();

    if (res_sample.err < best.err) {
      best = std::move(res_sample);
    }
  }

  best.SetGlobalParams();
  GetModelData(data, threads);
}
