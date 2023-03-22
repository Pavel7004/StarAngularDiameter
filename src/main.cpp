#include <cmath>
#include <iostream>

const double pi = acos(-1.0); // Число е
const double I0 = 5;        // Интенсивность свечения
const double Radius = 24.0;   // Радиус апетуры
const double lyambda1 = 6250; // Интервал(начало) длины волны
const double lyambda2 = 7750; // Интервал(конец) длины волны
const double l = 3.64825e8; // Расстояние до Луны
const double P1 = 1.0;      // хз
const double P2 = 0.0;      // хз
const double L0 = 2.0;      // хз
const double deltat = 2e-3; // Шаг данных t ???
const double V = 726.0;     // Скорость центра диска луны
const double t0 = 7.84e-2;  // Начальное t
const double R0 = 2.8;      // Радиус проекции звезды
                       // на плоскость видимого диска луны


double sigma(double y) { return 2.0 * sqrt(Radius * Radius - y * y); }

double S(double omega) {
  return omega/6.0*(4*sin(pi*omega*omega/8.0)+sin(pi*omega*omega/2.0));
}

double C(double omega) {
  return omega/6.0*(1+4*cos(pi*omega*omega/8.0)+cos(pi*omega*omega/2.0));
}

double G0(double x) {
  return I0 / 8.0 *
         (2.0 * x + 4.0 * x * C(x) - 4.0 / pi * sin(pi / 2.0 * x * x) +
          4.0 * x * S(x) + 4.0 / pi * cos(pi / 2.0 * x * x) +
          4.0 * x * C(x) * C(x) - 8.0 / pi * sin(pi / 2.0 * x * x) * C(x) +
          4.0 * x * S(x) * S(x) + 8.0 / pi * cos(pi / 2.0 * x * x) * S(x));
}

double g(double x, double lyambda) {
  return sqrt(lyambda * l / 2.0) * G0(x * sqrt(2.0 / (lyambda * l)));
}

double G1(double x) {
  return (lyambda2 - lyambda1) / 6.0 *
         (g(x, lyambda1) + 4 * g(x, (lyambda2 + lyambda1) / 2.0) +
          g(x, lyambda2));
}

double G2(double x) { return 8.0 * Radius * Radius * G1(x) / 3.0; }

double G3(double x) { return 4.0 * R0 * R0 / 3.0 * G2(x); }

double G4(double x) { return 4.0 * R0 * R0 * G2(x)/3.0; }

double T1(double t) {
  return (G3(V * (t + deltat - t0)) - G3(V * (t - deltat - t0))) / V;
}

double T2(double t) {
  return (G4(V * (t + deltat - t0)) - G4(V * (t - deltat - t0))) / V;
}

double T(double t) {
  return P1 * T1(t) + P2 * T2(t) + L0;
}

int main() {
  const size_t steps = 30'000'000;
  double t = t0;
  double dt = deltat;

  for (size_t i = 0; i < steps; ++i) {
    printf("%.10e %.10e\n", t, T(t));
    t += dt;
  }

  return 0;
}
