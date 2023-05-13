#ifndef MATRIX_H_
#define MATRIX_H_

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

  [[nodiscard]] consteval std::size_t GetHeight() const noexcept;
  [[nodiscard]] consteval std::size_t GetWidth() const noexcept;

  [[nodiscard]] inline value_type& operator()(
      const std::size_t& idx_x, const std::size_t& idx_y) noexcept;

  [[nodiscard]] inline value_type operator()(
      const std::size_t& idx_x, const std::size_t& idx_y) const noexcept;

  template <std::size_t MxN, std::size_t MxM>
  [[nodiscard]] inline Matrix<N, MxM> operator*(
      Matrix<MxN, MxM> const& mx) const noexcept;

  [[nodiscard]] inline Matrix<N, 1> operator*(
      std::vector<value_type> const& vec) const noexcept;

  [[nodiscard]] inline Matrix operator+(Matrix const& mx) const noexcept;
  [[nodiscard]] inline Matrix operator-(Matrix const& mx) const noexcept;

  [[nodiscard]] inline Matrix<M, N> Transpose() const noexcept;

  [[nodiscard]] inline Matrix Cholecky() const noexcept;

 private:
  std::array<value_type, N * M> data_;
};

#endif  // MATRIX_H_
