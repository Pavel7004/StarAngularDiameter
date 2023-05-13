#include "matrix.h"

#include <cmath>
#include <cstddef>
#include <vector>

template <std::size_t N, std::size_t M>
inline typename Matrix<N, M>::value_type& Matrix<N, M>::operator()(
    const std::size_t& idx_x, const std::size_t& idx_y) noexcept {
  return data_[idx_y * M + idx_x];
}

template <std::size_t N, std::size_t M>
inline typename Matrix<N, M>::value_type Matrix<N, M>::operator()(
    const std::size_t& idx_x, const std::size_t& idx_y) const noexcept {
  return data_.at(idx_y * M + idx_x);
}

template <std::size_t N, std::size_t M>
consteval std::size_t Matrix<N, M>::GetHeight() const noexcept {
  return N;
}

template <std::size_t N, std::size_t M>
consteval std::size_t Matrix<N, M>::GetWidth() const noexcept {
  return M;
}

template <std::size_t N, std::size_t M>
template <std::size_t MxN, std::size_t MxM>
inline Matrix<N, MxM> Matrix<N, M>::operator*(
    Matrix<MxN, MxM> const& mx) const noexcept {
  static_assert(M == MxN, "Can't multiply matrix with this dimentions");

  Matrix<N, MxM> res;

  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < mx.GetWidth(); ++j) {
      for (std::size_t k = 0; k < M; ++k) {
        res(i, j) += (*this)(i, k) * mx(k, j);
      }
    }
  }

  return res;
}

template <std::size_t N, std::size_t M>
inline Matrix<N, 1> Matrix<N, M>::operator*(
    std::vector<typename Matrix<N, M>::value_type> const& vec) const noexcept {
  Matrix<N, 1> res;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < vec.size(); ++j) {
      res(i, 0) += (*this)(i, j) * vec[j];
    }
  }

  return res;
}

template <std::size_t N, std::size_t M>
inline Matrix<N, M> Matrix<N, M>::operator+(
    Matrix<N, M> const& mx) const noexcept {
  Matrix<N, M> res;

  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < M; ++j) {
      res(i, j) = (*this)(i, j) + mx(i, j);
    }
  }

  return res;
}

template <std::size_t N, std::size_t M>
inline Matrix<N, M> Matrix<N, M>::operator-(
    Matrix<N, M> const& mx) const noexcept {
  Matrix<N, M> res;

  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < M; ++j) {
      res(i, j) = (*this)(i, j) - mx(i, j);
    }
  }

  return res;
}

template <std::size_t N, std::size_t M>
inline Matrix<M, N> Matrix<N, M>::Transpose() const noexcept {
  Matrix<M, N> tr;

  for (std::size_t i = 0; i < M; ++i) {
    for (std::size_t j = 0; j < N; ++j) {
      tr(i, j) = (*this)(j, i);
    }
  }

  return tr;
}

template <std::size_t N, std::size_t M>
inline Matrix<N, M> Matrix<N, M>::Cholecky() const noexcept {
  Matrix<N, M> l;

  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j <= i; ++j) {
      value_type sum = 0;
      for (std::size_t k = 0; k < j; ++k) {
        sum += l(i, k) * l(j, k);
      }

      l(i, j) = (i == j) ? std::sqrt((*this)(i, i) - sum)
                         : (1.0 / l(j, j) * ((*this)(i, j) - sum));
    }
  }

  return l;
}
