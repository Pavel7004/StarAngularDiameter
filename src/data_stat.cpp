#include "data_stat.h"

#include <absl/random/bit_gen_ref.h>
#include <absl/random/random.h>
#include <fmt/core.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <future>
#include <queue>
#include <vector>

#include "constants.h"
#include "datavec.h"
#include "theory.h"

namespace {
DataSet generateInputData(DataSet const& set, absl::BitGenRef gen) {
  constexpr double kRandRange = 0.10;

  const double t0_range = set.t0_ * kRandRange;
  const double l0_range = set.l0_ * kRandRange;
  const double b0_range = set.b0_ * kRandRange;
  const double r0_range = set.r0_ * kRandRange;

  return DataSet(set.t0_ + absl::Uniform(gen, -t0_range, t0_range),
                 set.l0_ + absl::Uniform(gen, -l0_range, l0_range),
                 set.b0_ + absl::Uniform(gen, -b0_range, b0_range),
                 set.r0_ + absl::Uniform(gen, -r0_range, r0_range), 1e20);
}

std::vector<DataSet> monteCarloWorker(DataArray data, const DataSet in_data,
                                      const std::size_t passes,
                                      const std::size_t threads) {
  absl::BitGen gen;

  std::size_t fixed_size = (passes < 100) ? passes : 100;
  std::vector<DataSet> results_heap;
  results_heap.reserve(fixed_size + 1);

  for (std::size_t i = 0; i < passes; ++i) {
    auto sample = generateInputData(in_data, gen);

    GetModelData(data, sample, threads);
    sample.err = ComputeSqErr(data);

    results_heap.push_back(std::move(sample));
    std::push_heap(results_heap.begin(), results_heap.end(), std::greater<>());

    if (results_heap.size() > fixed_size) {
      results_heap.pop_back();
    }
  }

  return results_heap;
}

}  // namespace

double ComputeSqErr(DataArray const& data) {
  return std::sqrt(
      (std::abs(data.N_model * data.N_model - data.N_data * data.N_data))
          .sum() /
      static_cast<double>(data.N_model.size()));
}

std::vector<DataSet> ApplyMonteKarlo(DataArray& data, DataSet const& params,
                                     std::size_t passes,
                                     const std::size_t threads) {
  // NOTE: number of threads to create beside main one
  const std::size_t rand_threads = threads - 1;
  const std::size_t calc_threads = 1;

  passes = passes / (rand_threads + 1);

  std::vector<std::future<std::vector<DataSet>>> results(rand_threads);
  for (auto& res : results) {
    res = std::async(std::launch::async, monteCarloWorker, data, params, passes,
                     calc_threads);
  }

  auto run_data = monteCarloWorker(data, params, passes, calc_threads);
  run_data.reserve(run_data.size() * threads + 1);
  run_data.push_back(params);
  for (auto& res : results) {
    auto run_sample = res.get();

    run_data.insert(run_data.end(), run_sample.begin(), run_sample.end());
  }

  std::sort(run_data.begin(), run_data.end());

  return run_data;
}

std::array<double, 2> GetValueRange(std::vector<DataSet> const& data) {
  double min = data.at(0).r0_;
  double max = min;

  for (auto const& el : data) {
    min = std::min(min, el.r0_);
    max = std::max(max, el.r0_);
  }

  return {min, max};
}
