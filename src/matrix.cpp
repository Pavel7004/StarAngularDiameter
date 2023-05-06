#include "matrix.h"
#include <cmath>

Matrix::Matrix(const std::size_t& height, const std::size_t& width) noexcept
    : height_(height), width_(width) {
  data_.reserve(height * width);
}

Matrix::value_type Matrix::Get(const std::size_t& idx_x,
                               const std::size_t& idx_y) const noexcept {
  return data_[idx_y * width_ + idx_x];
}

Matrix::value_type& Matrix::get_ref(const std::size_t& idx_x,
                                    const std::size_t& idx_y) noexcept {
  return data_[idx_y * width_ + idx_x];
}

Matrix Matrix::Cholecky() const noexcept {
  Matrix l(height_, width_);

  for (std::size_t i = 0; i < height_; ++i) {
    for (std::size_t j = 0; j <= i; ++j) {
      value_type sum = 0;
      for (std::size_t k = 0; k < j; ++k)
        sum += l.Get(i, k) * l.Get(j, k);

      auto& cell = l.get_ref(i, j);
      if (i == j)
        cell = std::sqrt(this->Get(i, i) - sum);
      else
        cell = (1.0 / l.Get(j, j) * (this->Get(i, j) - sum));
    }
  }

  return l;
}
