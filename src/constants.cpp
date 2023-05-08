#include "constants.h"

const double I0 = 4.509;  // Интенсивность свечения (Вт/м^2)
const double R = 0.24;  // Радиус апетуры (м)
const double lambda1 = 6250e-10;  // Интервал(начало) длины волны (м)
const double lambda2 = 7750e-10;  // Интервал(конец) длины волны (м)
const double l = 3.64825e8;  // Расстояние до Луны (м)
const double deltat = 2e-3;  // Шаг данных t (с)
const double V = 726.0;  // Скорость центра диска луны (м/с)
const double R0 =
    2.8;  // Радиус проекции звезды на плоскость видимого диска луны (м)

thread_local double t0 = 212.4e-3;  // Время пересечения центра диска луны (с)
thread_local double L0 = 2125.0;  // Фон неба
thread_local double B0 = 2.9e11;  // Яркость в центре диска луны
thread_local double m = 0.4;  // Коэфицент потемнения в линейном законе
