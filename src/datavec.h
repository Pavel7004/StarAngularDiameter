#ifndef STAR_DATAVEC_
#define STAR_DATAVEC_

#include <valarray>

struct DataArray {
  explicit DataArray(std::valarray<double> t, std::valarray<double> x,
                     std::valarray<double> N_model,
                     std::valarray<double> N_data)
      : t(std::move(t)),
        x(std::move(x)),
        N_model(std::move(N_model)),
        N_data(std::move(N_data)) {}

  ~DataArray() = default;

  DataArray(const DataArray&) = delete;
  DataArray(DataArray&&) = default;
  DataArray& operator=(const DataArray&) = delete;
  DataArray& operator=(DataArray&&) = default;

  std::valarray<double> t;
  std::valarray<double> x;
  std::valarray<double> N_model;
  std::valarray<double> N_data;
};

#endif  // STAR_DATAVEC_
