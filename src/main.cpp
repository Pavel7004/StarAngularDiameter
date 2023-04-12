#include <cmath>
#include <functional>
#include <iostream>
#include <numbers>

// clang-format off

constexpr double I0       = 4.509;         // Интенсивность свечения (Вт/м^2)
constexpr double R        = 0.24;          // Радиус апетуры (м)
constexpr double lambda1  = 6250e-10;      // Интервал(начало) длины волны (м)
constexpr double lambda2  = 7750e-10;      // Интервал(конец) длины волны (м)
constexpr double l        = 3.64825e8;     // Расстояние до Луны (м)
constexpr double P1       = 1.0;           // Распределение яркости по диску звезды
constexpr double P2       = 0.0;           // --//--
constexpr double L0       = 2.0;           // Фон неба
constexpr double deltat   = 2e-3;          // Шаг данных t (с)
constexpr double V        = 726.0;         // Скорость центра диска луны (м/с)
constexpr double t0       = 78.4e-3;       // Время пересечения центра диска луны (с)
constexpr double tN       = 180e-3;        // Время наблюдений (с)
constexpr double R0       = 2.8;           // Радиус проекции звезды на плоскость видимого диска луны (м)

// clang-format on

using std::cos;
using std::sin;
using std::sqrt;
using std::numbers::pi; // Число е

double simpson(const std::function<double(const double &)> &f,
               const double &from, const double &to) {
  constexpr std::size_t parts = 30;
  const double width = (to - from) / parts;

  double res = 0.0;

#pragma GCC unroll parts
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
  return I0 / 8.0 *
         (2.0 * x + 4.0 * x * C(x) - 4.0 / pi * sin(pi * x * x / 2.0) +
          4.0 * x * S(x) + 4.0 / pi * cos(pi * x * x / 2.0) +
          4.0 * x * C(x) * C(x) - 8.0 / pi * sin(pi * x * x / 2.0) * C(x) +
          4.0 * x * S(x) * S(x) + 8.0 / pi * cos(pi * x * x / 2.0) * S(x));
}

inline double G1(const double &x) {
  return simpson(
      [&x](const double &lambda) {
        return sqrt(lambda * l / 2.0) * G0(x * sqrt(2.0 / (l * lambda)));
      },
      lambda1, lambda2);
}

inline double G2(const double &x) {
  return simpson([&x](const double &y) { return sigma(y) * G1(x + y); }, -R, R);
}

inline double G3(const double &x) {
  return simpson(
      [&x](const double &beta) {
        return sqrt(R0 * R0 - beta * beta) * G2(x + beta);
      },
      -R0, R0);
}

inline double G4(const double &x) {
  return simpson(
      [&x](const double &beta) {
        return (R0 * R0 - beta * beta) / R0 * G2(x + beta);
      },
      -R0, R0);
}

inline double T1(const double &t) {
  return (G3(V * (t + deltat - t0)) - G3(V * (t - deltat - t0))) / V;
}

inline double T2(const double &t) {
  return (G4(V * (t + deltat - t0)) - G4(V * (t - deltat - t0))) / V;
}

inline double T(const double &t) { return P1 * T1(t) + P2 * T2(t) + L0; }

int main(void) {
  for (double t = tN; t > 0; t -= 2 * deltat) {
    printf("%.10e %.10e\n", V * (t0 - t), T(t));
  }

  return EXIT_SUCCESS;
}
