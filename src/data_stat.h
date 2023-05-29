#ifndef DATA_STAT_H_
#define DATA_STAT_H_

#include "constants.h"
#include "datavec.h"
#include "theory.h"

double ComputeSqErr(DataArray const& data);
std::vector<DataSet> ApplyMonteKarlo(DataArray& data, DataSet const& params,
                                     std::size_t passes, std::size_t threads);
std::array<double, 2> GetValueRange(std::vector<DataSet> const& data);

#endif  // DATA_STAT_H_
