#include "matrix.h"

constexpr Matrix::Matrix(const std::size_t& height,
                         const std::size_t& width) noexcept
    : height_(height), width_(width) {
  data_.reserve(height * width);
}

Matrix::value_type& Matrix::operator[](const std::size_t& idx_x,
                                       const std::size_t& idx_y) noexcept {
  return data_[idx_y * width_ + idx_x];
}
