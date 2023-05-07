#include "data_stat.h"

#include <cmath>

double ComputeSqErr(const DataArray& data) {
  return std::sqrt(
      (std::abs(data.N_model * data.N_model - data.N_data * data.N_data))
          .sum());
}
