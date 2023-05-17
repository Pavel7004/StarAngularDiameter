#ifndef MATRIX_H_
#define MATRIX_H_

#include <array>
#include <cmath>
#include <cstddef>
#include <vector>

template <std::size_t N, std::size_t M>
class Matrix {
  using value_type = double;

 public:
  explicit constexpr Matrix() noexcept = default;
  ~Matrix() noexcept = default;

  Matrix(const Matrix&) = delete;
  Matrix& operator=(const Matrix&) = delete;

  Matrix(Matrix&&) noexcept = default;
  Matrix& operator=(Matrix&&) noexcept = default;

  [[nodiscard]] static Matrix<N, N> IdentityMatrix() noexcept {
    Matrix<N, N> e;

    e.data_ = {0};
    for (std::size_t i = 0; i < N; ++i) {
      e(i, i) = 1;
    }

    return e;
  }

  [[nodiscard]] constexpr std::size_t GetHeight() const noexcept { return N; }

  [[nodiscard]] constexpr std::size_t GetWidth() const noexcept { return M; }

  [[nodiscard]] inline value_type& operator()(
      const std::size_t& idx_x, const std::size_t& idx_y) noexcept {
    return data_.at(idx_x * M + idx_y);
  }

  [[nodiscard]] inline value_type operator()(
      const std::size_t& idx_x, const std::size_t& idx_y) const noexcept {
    return data_.at(idx_x * M + idx_y);
  }

  template <std::size_t MxN, std::size_t MxM>
    requires(M == MxN)
  [[nodiscard]] Matrix<N, MxM> operator*(
      Matrix<MxN, MxM> const& mx) const noexcept {
    Matrix<N, MxM> res;

    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < MxM; ++j) {
        res(i, j) = 0;
        for (std::size_t k = 0; k < M; ++k) {
          res(i, j) += (*this)(i, k) * mx(k, j);
        }
      }
    }

    return res;
  }

  [[nodiscard]] std::array<value_type, N> operator*(
      std::array<value_type, M> const& arr) const noexcept {
    std::array<value_type, N> res;

    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < M; ++j) {
        res[i] += (*this)(i, j) * arr[j];
      }
    }

    return res;
  }

  [[nodiscard]] Matrix operator+(Matrix const& mx) const noexcept {
    Matrix<N, M> res;

    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < M; ++j) {
        res(i, j) = (*this)(i, j) + mx(i, j);
      }
    }

    return res;
  }

  [[nodiscard]] Matrix operator-(Matrix const& mx) const noexcept {
    Matrix<N, M> res;

    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < M; ++j) {
        res(i, j) = (*this)(i, j) - mx(i, j);
      }
    }

    return res;
  }

  [[nodiscard]] Matrix<M, N> Transpose() const noexcept {
    Matrix<M, N> tr;

    for (std::size_t i = 0; i < M; ++i) {
      for (std::size_t j = 0; j < N; ++j) {
        tr(i, j) = (*this)(j, i);
      }
    }

    return tr;
  }

  [[nodiscard]] Matrix<N, N> Cholecky() const noexcept {
    static_assert(N == M, "Cholecky can't be called on non square matrixes.");
    Matrix<N, N> l;

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

 private:
  std::array<value_type, N * M> data_;
};

#endif  // MATRIX_H_
