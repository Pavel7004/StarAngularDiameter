#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>

// clang-format off

const double pi = acos(-1.0); // Число е

constexpr double I0       = 5.49;        // Интенсивность свечения
constexpr double Radius   = 24.0;        // Радиус апетуры
constexpr double lyambda1 = 6250;        // Интервал(начало) длины волны
constexpr double lyambda2 = 7750;        // Интервал(конец) длины волны
constexpr double l        = 3.64825e8;   // Расстояние до Луны
constexpr double P1       = 1.0;         // хз
constexpr double P2       = 0.0;         // хз
constexpr double L0       = 2.0;         // Фон неба
constexpr double deltat   = 2e-3;        // Шаг данных t 
constexpr double V        = 726.0;       // Скорость центра диска луны
constexpr double t0       = 7.84e-2;     // Начальное t
constexpr double tN       = t0 + 22e-1;  // Время проведения измерений
constexpr double R0       = 2.8;         // Радиус проекции звезды на плоскость видимого диска луны

// clang-format on

inline double simpson(const std::function<double(double)> &f,
                      const double &from, const double &to) {
  const double middle = from + (to - from) / 2;
  return (to - from) / 6 * (f(from) + 4 * f(middle) + f(to));
}

inline double sigma(const double &y) {
  return 2.0 * sqrt(Radius * Radius - y * y);
}

inline double S(const double &omega) {
  return omega / 6.0 *
         (4 * sin(pi * omega * omega / 8.0) + sin(pi * omega * omega / 2.0));
}

inline double C(const double &omega) {
  return omega / 6.0 *
         (1 + 4 * cos(pi * omega * omega / 8.0) +
          cos(pi * omega * omega / 2.0));
}

inline double G0(const double &x) {
  return I0 / 8.0 *
         (2.0 * x + 4.0 * x * C(x) - 4.0 / pi * sin(pi / 2.0 * x * x) +
          4.0 * x * S(x) + 4.0 / pi * cos(pi / 2.0 * x * x) +
          4.0 * x * C(x) * C(x) - 8.0 / pi * sin(pi / 2.0 * x * x) * C(x) +
          4.0 * x * S(x) * S(x) + 8.0 / pi * cos(pi / 2.0 * x * x) * S(x));
}

inline double g(const double &x, const double &lyambda) {
  return sqrt(lyambda * l / 2.0) * G0(x * sqrt(2.0 / (lyambda * l)));
}

inline double G1(const double &x) {
  return (lyambda2 - lyambda1) / 6.0 *
         (g(x, lyambda1) + 4 * g(x, (lyambda2 + lyambda1) / 2.0) +
          g(x, lyambda2));
}

inline double G2(const double &x) {
  return 8.0 * Radius * Radius * G1(x) / 3.0;
}

inline double G3(const double &x) { return 4.0 * R0 * R0 / 3.0 * G2(x); }

inline double G4(const double &x) { return 4.0 * R0 * R0 * G2(x) / 3.0; }

inline double T1(const double &t) {
  return (G3(V * (t + deltat - t0)) - G3(V * (t - deltat - t0))) / V;
}

inline double T2(const double &t) {
  return (G4(V * (t + deltat - t0)) - G4(V * (t - deltat - t0))) / V;
}

inline double T(const double &t) { return P1 * T1(t) + P2 * T2(t) + L0; }

int main() {
  double t = t0;

  while (t < tN) {
    printf("%.10e %.10e\n", t, T(t));
    t += 2 * deltat;
  }

  return EXIT_SUCCESS;
}
