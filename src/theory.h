#ifndef STAR_THEORY_
#define STAR_THEORY_

#include "cache.h"
#include "constants.h"
#include "datavec.h"

double T(const double& t);

void DeleteCache();
void NewCache();

void GetModelData(DataArray& data, DataSet const& params,
                  std::size_t thread_count);
void GetCordsData(DataArray& data, DataSet const& params);

#endif  // STAR_THEORY_
