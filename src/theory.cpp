#include "theory.h"
#include "cache.h"
#include "constants.h"
#include <cmath>
#include <cstdio>
#include <functional>
#include <future>

namespace {

#include <numbers>

using std::cos;
using std::sin;
using std::sqrt;
using std::numbers::pi; // Число е

thread_local Cache cache;
thread_local std::size_t id_G0, id_G1, id_G2, id_G3, id_G4;

template <std::size_t parts = 34>
double simpson(const std::function<double(const double &)> &f,
               const double &from, const double &to) {
  const double width = (to - from) / parts;

  double res = 0.0;
  for (std::size_t step = 0; step < parts; ++step) {
    const double x1 = from + step * width;
    const double x2 = from + (step + 1) * width;

    res += (x2 - x1) / 6.0 * (f(x1) + 4.0 * f(0.5 * (x1 + x2)) + f(x2));
  }

  return res;
}

inline double sigma(const double &y) { return 2.0 * sqrt(R * R - y * y); }

inline double S(const double &omega) {
  return simpson([](const double &t) { return sin(pi * t * t / 2.0); }, 0.0,
                 omega);
}

inline double C(const double &omega) {
  return simpson([](const double &t) { return cos(pi * t * t / 2.0); }, 0.0,
                 omega);
}

inline double G0(const double &x) {
  const double c = C(x), s = S(x);
  return I0 / 8.0 *
         (2.0 * x + 4.0 * x * c - 4.0 / pi * sin(pi * x * x / 2.0) +
          4.0 * x * s + 4.0 / pi * cos(pi * x * x / 2.0) + 4.0 * x * c * c -
          8.0 / pi * sin(pi * x * x / 2.0) * c + 4.0 * x * s * s +
          8.0 / pi * cos(pi * x * x / 2.0) * s);
}

inline double G1(const double &x) {
  return simpson<60>(
      [&x](const double &lambda) {
        return sqrt(lambda * l / 2.0) *
               cache.GetFunctionValue(id_G0, x * sqrt(2.0 / (l * lambda)));
      },
      lambda1, lambda2);
}

inline double G2(const double &x) {
  return simpson<60>(
      [&x](const double &y) {
        return sigma(y) * cache.GetFunctionValue(id_G1, x + y);
      },
      -R, R);
}

inline double G3(const double &x) {
  return simpson<60>(
      [&x](const double &beta) {
        return sqrt(R0 * R0 - beta * beta) *
               cache.GetFunctionValue(id_G2, x + beta);
      },
      -R0, R0);
}

inline double G4(const double &x) {
  return simpson<60>(
      [&x](const double &beta) {
        return (R0 * R0 - beta * beta) / R0 *
               cache.GetFunctionValue(id_G2, x + beta);
      },
      -R0, R0);
}

inline double T1(const double &t) {
  return (cache.GetFunctionValue(id_G3, V * (t + deltat - t0)) -
          cache.GetFunctionValue(id_G3, V * (t - deltat - t0))) /
         V;
}

inline double T2(const double &t) {
  return (cache.GetFunctionValue(id_G4, V * (t + deltat - t0)) -
          cache.GetFunctionValue(id_G4, V * (t - deltat - t0))) /
         V;
}

inline double T(const double &t) { return P1 * T1(t) + P2 * T2(t) + L0; }

} // namespace

datavec getData(const double from, const double to) {
  // const double maxT = T(t0 + 50.0 / V), minT = T(t0 - 40.0 / V);
  cache = Cache();

  id_G0 = cache.RegisterFunction(G0);
  id_G1 = cache.RegisterFunction(G1);
  id_G2 = cache.RegisterFunction(G2);
  id_G3 = cache.RegisterFunction(G3);
  id_G4 = cache.RegisterFunction(G4);

  thread_local datavec data;
  data.reserve(static_cast<std::size_t>((to - from) / (2 * deltat)));

  double t = to;
  while (t > from) {
    // data.emplace_back(V * (t0 - t), (T(t) - minT) / (maxT - minT));
    data.emplace_back(V * (t0 - t), T(t));

    t -= 2 * deltat;
  }

  // cache.GetHitInfo();

  return data;
}

datavec GetData(const double &from, const double &to,
                const std::size_t &thread_count) {
  const double width = (to - from) / thread_count;

  std::vector<std::future<datavec>> results;
  results.reserve(thread_count);
  for (std::size_t thread = 0; thread < thread_count; ++thread) {
    const double x1 = from + thread * width;
    const double x2 = from + (thread + 1) * width;

    fprintf(stderr, "Thread %lu: %.5e -> %.5e\n", thread + 1, x1, x2);

    results.emplace_back(std::async(std::launch::async, getData, x1, x2));
  }

  datavec ret = results[0].get();
  std::reverse(ret.begin(), ret.end());

  for (std::size_t i = 1; i < results.size(); ++i) {
    datavec tmp = results[i].get();

    ret.reserve(ret.size() + tmp.size());
    ret.insert(ret.end(), tmp.rbegin(), tmp.rend());
  }

  std::stable_sort(ret.begin(), ret.end(), [](auto &left, auto &right) {
    return left.first < right.first;
  });

  return ret;
}
