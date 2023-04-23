#ifndef __STAR_THEORY__
#define __STAR_THEORY__

#include <vector>

using datavec = std::vector<std::pair<double, double>>;

datavec GetData(const double& from, const double& to,
                const std::size_t& thread_count);

#endif  // __STAR_THEORY__
