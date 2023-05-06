#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>

class Matrix {
  using value_type = double;

 public:
  explicit Matrix(const std::size_t& height, const std::size_t& width) noexcept;
  ~Matrix() noexcept = default;

  [[nodiscard]] value_type Get(const std::size_t& idx_x,
                               const std::size_t& idx_y) const noexcept;

  [[nodiscard]] Matrix Cholecky() const noexcept;

 private:
  value_type& get_ref(const std::size_t& idx_x,
                      const std::size_t& idx_y) noexcept;

  std::size_t height_;
  std::size_t width_;

  std::vector<value_type> data_;
};

#endif  // MATRIX_H_
