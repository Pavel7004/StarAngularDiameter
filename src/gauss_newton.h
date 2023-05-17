#ifndef STAR_GAUSS_NEWTON_
#define STAR_GAUSS_NEWTON_

#include <cstddef>

#include "datavec.h"

void ApplyGaussNewton(DataArray const& data, std::size_t iterations);

#endif  // STAR_GAUSS_NEWTON_
