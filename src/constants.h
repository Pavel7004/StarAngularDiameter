#ifndef STAR_CONSTANTS_
#define STAR_CONSTANTS_

// clang-format off

extern const double I0;       // Интенсивность свечения (Вт/м^2)
extern const double R;        // Радиус апетуры (м)
extern const double lambda1;  // Интервал(начало) длины волны (м)
extern const double lambda2;  // Интервал(конец) длины волны (м)
extern const double l;        // Расстояние до Луны (м)
extern const double deltat;   // Шаг данных t (с)
extern const double V;        // Скорость центра диска луны (м/с)
extern const double m;  // Коэфицент потемнения в линейном законе

extern thread_local double t0; // Время пересечения центра диска луны (с)
extern thread_local double L0; // Фон неба
extern thread_local double B0; // Яркость в центре диска луны
extern thread_local double R0; // Радиус проекции звезды на плоскость видимого диска луны (м)

// clang-format on

#endif  // STAR_CONSTANTS_
