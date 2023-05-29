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

// clang-format on

struct DataSet {
  explicit DataSet(double t0, double L0, double B0, double R0, double err)
      : t0_(t0), l0_(L0), b0_(B0), r0_(R0), err(err) {}

  ~DataSet() = default;

  DataSet(const DataSet&) = default;
  DataSet(DataSet&&) = default;
  DataSet& operator=(const DataSet&) = default;
  DataSet& operator=(DataSet&&) = default;

  double t0_;
  double l0_;
  double b0_;
  double r0_;
  double err;

  inline bool operator>(DataSet const& other) const noexcept {
    return this->err > other.err;
  }

  inline bool operator<(DataSet const& other) const noexcept {
    return this->err < other.err;
  }
};

#endif  // STAR_CONSTANTS_
