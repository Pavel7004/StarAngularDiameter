#ifndef __STAR_CONSTANTS__
#define __STAR_CONSTANTS__

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

#endif // __STAR_CONSTANTS__
