#ifndef STAR_DATAVEC_
#define STAR_DATAVEC_

#include <valarray>

struct DataArray {
  std::valarray<double> t;
  std::valarray<double> x;
  std::valarray<double> N_model;
  std::valarray<double> N_data;
};

#endif  // STAR_DATAVEC_
