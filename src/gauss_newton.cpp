#include "gauss_newton.h"

#include <fmt/core.h>
#include <array>
#include <cstddef>
#include <vector>

#include "cache.h"
#include "constants.h"
#include "datavec.h"
#include "matrix.h"
#include "theory.h"

namespace {

template <std::size_t N>
Matrix<N, N> r(Matrix<N, N> const& A) {
  auto rA = Matrix<N, N>::IdentityMatrix();

  const auto l = A.Cholecky();
  for (std::size_t i = 0; i < N; ++i) {
    rA(i, 0) *= (1.0 / l(i, i));
    for (std::size_t j = i + 1; j < N; ++j) {
      rA(j, 0) -= rA(i, 0) * l(j, i);
    }
  }

  return rA.Transpose() * rA;  // NOTE: недописал
}

template <std::size_t N>
double f(double x, std::array<double, N> const& B) {
  t0 = B[0];
  B0 = B[1];
  L0 = B[2];
  R0 = B[3];
  return T(x);
}

template <std::size_t N>
double rr(std::size_t x, DataArray const& data,
          std::array<double, N> const& B) {
  return f(data.t[x], B) - data.N_data[x];
}

template <std::size_t N>
std::array<double, N> df(double x, std::array<double, N> const& B,
                         DataArray const& data) {
  const auto rn = [&](std::array<double, N> const& B) -> double {
    return rr<N>(x, data, B);
  };

  constexpr double dt = 100;
  const double err = rn(B);

  const double dx =
      (rn({B[0] + B[0] / dt, B[1], B[2], B[3]}) - err) / (B[0] / dt);
  const double dy =
      (rn({B[0], B[1] + B[1] / dt, B[2], B[3]}) - err) / (B[1] / dt);
  const double dz =
      (rn({B[0], B[1], B[2] + B[2] / dt, B[3]}) - err) / (B[2] / dt);
  const double dk =
      (rn({B[0], B[1], B[2], B[3] + B[3] / dt}) - err) / (B[3] / dt);

  return {dx, dy, dz, dk};
}

template <std::size_t N, std::size_t M>
Matrix<N, M> Jacobian(DataArray const& data, std::array<double, M> const& B) {
  Matrix<N, M> jb;

  for (std::size_t i = 0; i < N; ++i) {
    const auto row = df(i, B, data);
    for (std::size_t j = 0; j < M; ++j) {
      jb(i, j) = row[j];
    }
  }

  return jb;
}

template <std::size_t N, std::size_t M>
std::array<double, N> rR(DataArray const& data,
                         std::array<double, M> const& B) {
  std::array<double, N> RR;

  for (std::size_t i = 0; i < M; ++i) {
    RR[i] = f(data.t[i], B) - data.N_data[i];
  }

  return RR;
}

template <std::size_t N>
double sr(DataArray const& data, std::array<double, N> const& B) {
  double R = 0;

  for (std::size_t i = 0; i < data.t.size(); ++i) {
    R += fabs(data.N_data[i] * data.N_data[i] -
              f(data.t[i], B) * f(data.t[i], B));
  }

  return R;
}

template <typename T, std::size_t N>
std::array<T, N> operator-(std::array<T, N> const& left,
                           std::array<T, N> const& right) {
  std::array<T, N> res;
  for (std::size_t i = 0; i < N; ++i) {
    res[i] = left[i] - right[i];
  }
  return res;
}

template <std::size_t N, std::size_t M>
void printMtx(Matrix<N, M> const& mx) {
  for (std::size_t i = 0; i < mx.GetHeight(); ++i) {
    for (std::size_t j = 0; j < mx.GetWidth(); ++j) {
      fmt::print(stderr, "{} ", mx(i, j));
    }
    fmt::print(stderr, "\n");
  }
}

}  // namespace

void ApplyGaussNewton(DataArray const& data, std::size_t iterations) {
  constexpr std::size_t kDataSetSize = 200;

  std::array<double, 4> B = {t0, B0, L0, R0};

  NewCache();

  for (std::size_t i = 0; i < iterations; ++i) {
    fmt::print(stderr, "{:.3e} {:.3e} {:.3e} {:.3e}\n", B[0], B[1], B[2], B[3]);

    const auto jacob = Jacobian<kDataSetSize, 4>(data, B);
    const auto jacob_tr = jacob.Transpose();

    B = B - r(jacob_tr * jacob) * jacob_tr * rR<kDataSetSize>(data, B);
  }

  DeleteCache();
}
