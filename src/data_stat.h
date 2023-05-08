#ifndef DATA_STAT_H_
#define DATA_STAT_H_

#include "datavec.h"

double ComputeSqErr(const DataArray& data);
void ApplyMonteKarlo(DataArray& data, std::size_t passes, std::size_t threads);

#endif  // DATA_STAT_H_
