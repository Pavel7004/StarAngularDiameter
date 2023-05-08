#include "theory.h"

#include <algorithm>
#include <cmath>
#include <future>
#include <numbers>
#include <span>
#include <vector>

#include "cache.h"
#include "constants.h"
#include "datavec.h"

namespace {

using std::cos;
using std::sin;
using std::sqrt;
using std::numbers::pi;  // Число е

thread_local Cache cache;
thread_local std::size_t id_g0, id_g1, id_g2, id_g3, id_g4;

template <std::size_t Parts = 15>
double simpson(auto&& f, const double& from, const double& to) {
  const double width = (to - from) / Parts;

  double res = 0.0;
  for (std::size_t step = 0; step < Parts; ++step) {
    const double x1 = from + static_cast<double>(step) * width;
    const double x2 = from + static_cast<double>(step + 1) * width;

    res += (x2 - x1) / 6.0 * (f(x1) + 4.0 * f(0.5 * (x1 + x2)) + f(x2));
  }

  return res;
}

inline double S(const double& omega) {
  return simpson<130>(
      [](const double& t) -> double { return sin(pi * t * t / 2.0); }, 0.0,
      omega);
}

inline double C(const double& omega) {
  return simpson<130>(
      [](const double& t) -> double { return cos(pi * t * t / 2.0); }, 0.0,
      omega);
}

inline double G0(const double& x) {
  const double c = C(x);
  const double s = S(x);
  return I0 / 8.0 *
         (2.0 * x + 4.0 * x * c - 4.0 / pi * sin(pi * x * x / 2.0) +
          4.0 * x * s + 4.0 / pi * cos(pi * x * x / 2.0) + 4.0 * x * c * c -
          8.0 / pi * sin(pi * x * x / 2.0) * c + 4.0 * x * s * s +
          8.0 / pi * cos(pi * x * x / 2.0) * s);
}

inline double G1(const double& x) {
  return simpson(
      [&x](const double& lambda) -> double {
        return sqrt(lambda * l / 2.0) *
               cache.GetFunctionValue(id_g0, x * sqrt(2.0 / (l * lambda)));
      },
      lambda1, lambda2);
}

inline double sigma(const double& y) {
  return 2.0 * sqrt(R * R - y * y);
}

inline double G2(const double& x) {
  return simpson(
      [&x](const double& y) -> double {
        return sigma(y) * cache.GetFunctionValue(id_g1, x + y);
      },
      -R, R);
}

inline double G3(const double& x) {
  return simpson(
      [&x](const double& beta) -> double {
        return sqrt(R0 * R0 - beta * beta) / R0 *
               cache.GetFunctionValue(id_g2, x + beta);
      },
      -R0, R0);
}

inline double G4(const double& x) {
  return simpson(
      [&x](const double& beta) -> double {
        return (R0 * R0 - beta * beta) / R0 *
               cache.GetFunctionValue(id_g2, x + beta);
      },
      -R0, R0);
}

inline double T1(const double& t) {
  return (cache.GetFunctionValue(id_g3, V * (t + deltat - t0)) -
          cache.GetFunctionValue(id_g3, V * (t - deltat - t0))) /
         V;
}

inline double T2(const double& t) {
  return (cache.GetFunctionValue(id_g4, V * (t + deltat - t0)) -
          cache.GetFunctionValue(id_g4, V * (t - deltat - t0))) /
         V;
}

inline double P1() {
  return B0 * 2 * (1 - m);
}

inline double P2() {
  return B0 * (m / 2.0) * (pi / R0);
}

inline double T(const double& t) {
  return P1() * T1(t) + P2() * T2(t) + L0;
}

template <typename TT>
std::vector<double> getModelData(std::span<TT> data) {
  id_g0 = cache.RegisterFunction(G0);
  id_g1 = cache.RegisterFunction(G1);
  id_g2 = cache.RegisterFunction(G2);
  id_g3 = cache.RegisterFunction(G3);
  id_g4 = cache.RegisterFunction(G4);

  std::vector<double> model;
  model.reserve(data.size());

  for (const auto& t : data) {
    model.emplace_back(T(t));
  }

  return model;
}

}  // namespace

void GetModelData(DataArray& data, std::size_t thread_count) {
  thread_count -= 1;

  const auto width = (thread_count != 0) ? data.t.size() / thread_count : 0;
  const auto parent_work =
      (thread_count != 0) ? data.t.size() % thread_count : data.t.size();

  double* begin = std::begin(data.t);
  double* end = begin;
  std::vector<std::future<std::vector<double>>> results(thread_count);
  for (auto& res : results) {
    begin = end;
    end = begin + width;

    std::span s(begin, end);
    res = std::async(std::launch::async, [s]() { return getModelData(s); });
  }
  begin = end;
  end = begin + parent_work;
  std::span s(begin, end);
  auto par_res = getModelData(s);

  std::vector<double> model;
  model.reserve(data.t.size());

  for (auto& res : results) {
    auto tmp = res.get();

    model.insert(model.end(), tmp.begin(), tmp.end());
  }
  model.insert(model.end(), par_res.begin(), par_res.end());

  std::reverse(model.begin(), model.end());

  data.N_model = {model.data(), model.size()};
}

void GetCordsData(DataArray& data) {
  data.x = V * (data.t - t0);
}
