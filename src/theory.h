#ifndef STAR_THEORY_
#define STAR_THEORY_

#include <vector>

using datavec = std::vector<std::pair<double, double>>;

datavec GetData(const double& from, const double& to,
                const std::size_t& thread_count);

#endif  // STAR_THEORY_
