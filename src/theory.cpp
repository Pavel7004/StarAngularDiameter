#include "theory.h"
#include <cmath>
#include <functional>
#include <future>
#include <numbers>
#include "cache.h"
#include "constants.h"

namespace {

using std::cos;
using std::sin;
using std::sqrt;
using std::numbers::pi;  // Число е

thread_local Cache cache;
thread_local std::size_t id_g0, id_g1, id_g2, id_g3, id_g4;

template <std::size_t Parts = 56>
double simpson(const std::function<double(const double&)>& f,
               const double& from, const double& to) {
  const double width = (to - from) / Parts;

  double res = 0.0;
  for (std::size_t step = 0; step < Parts; ++step) {
    const double x1 = from + static_cast<double>(step) * width;
    const double x2 = from + static_cast<double>(step + 1) * width;

    res += (x2 - x1) / 6.0 * (f(x1) + 4.0 * f(0.5 * (x1 + x2)) + f(x2));
  }

  return res;
}

inline double sigma(const double& y) {
  return 2.0 * sqrt(R * R - y * y);
}

inline double S(const double& omega) {
  return simpson(
      [](const double& t) -> double { return sin(pi * t * t / 2.0); }, 0.0,
      omega);
}

inline double C(const double& omega) {
  return simpson(
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
  return simpson<86>(
      [&x](const double& lambda) -> double {
        return sqrt(lambda * l / 2.0) *
               cache.GetFunctionValue(id_g0, x * sqrt(2.0 / (l * lambda)));
      },
      lambda1, lambda2);
}

inline double G2(const double& x) {
  return simpson<86>(
      [&x](const double& y) -> double {
        return sigma(y) * cache.GetFunctionValue(id_g1, x + y);
      },
      -R, R);
}

inline double G3(const double& x) {
  return simpson<86>(
      [&x](const double& beta) -> double {
        return sqrt(R0 * R0 - beta * beta) / R0 *
               cache.GetFunctionValue(id_g2, x + beta);
      },
      -R0, R0);
}

inline double G4(const double& x) {
  return simpson<86>(
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

inline double T(const double& t) {
  return P1 * T1(t) + P2 * T2(t) + L0;
}

}  // namespace

datavec getData(const double from, const double to) {
  // const double maxT = T(t0 + 50.0 / V), minT = T(t0 - 40.0 / V);
  id_g0 = cache.RegisterFunction(G0);
  id_g1 = cache.RegisterFunction(G1);
  id_g2 = cache.RegisterFunction(G2);
  id_g3 = cache.RegisterFunction(G3);
  id_g4 = cache.RegisterFunction(G4);

  datavec data;
  data.reserve(static_cast<std::size_t>((to - from) / (2 * deltat)));

  double t = to;
  while (t > from) {
    // data.emplace_back(V * (t0 - t), (T(t) - minT) / (maxT - minT));
    data.emplace_back((tN - t) * 1000, T(t));

    t -= 2 * deltat;
  }

  return data;
}

datavec GetData(const double& from, const double& to,
                const std::size_t& thread_count) {
  const double width = (to - from) / static_cast<double>(thread_count);

  std::vector<std::future<datavec>> results;
  results.reserve(thread_count);
  for (std::size_t thread = 0; thread < thread_count; ++thread) {
    const double x1 = from + static_cast<double>(thread) * width;
    const double x2 = from + static_cast<double>(thread + 1) * width;

    results.emplace_back(std::async(std::launch::async, getData, x1, x2));
  }

  datavec ret = results[0].get();
  std::reverse(ret.begin(), ret.end());
  ret.reserve(ret.size() * thread_count);
  results.erase(results.begin());

  for (auto& res : results) {
    datavec tmp = res.get();

    ret.insert(ret.end(), tmp.rbegin(), tmp.rend());
  }

  std::stable_sort(ret.begin(), ret.end(), [](auto& left, auto& right) -> bool {
    return left.first < right.first;
  });

  return ret;
}
