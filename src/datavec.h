#ifndef STAR_DATAVEC_
#define STAR_DATAVEC_

#include <vector>

struct DataArray {
  std::vector<double> t;
  std::vector<double> x;
  std::vector<double> N_model;
  std::vector<double> N_data;
};

#endif  // STAR_DATAVEC_
