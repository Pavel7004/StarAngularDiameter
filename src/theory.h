#ifndef STAR_THEORY_
#define STAR_THEORY_

#include "datavec.h"

double T(const double& t);

void GetModelData(DataArray& data, std::size_t thread_count);
void GetCordsData(DataArray& data);

#endif  // STAR_THEORY_
