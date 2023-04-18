#include "cache.h"
#include <cstdio>

std::size_t Cache::RegisterFunction(
    const std::function<double(const double &)> &f) noexcept {
  funcs.emplace_back(f);
  return funcs.size() - 1;
}

double Cache::GetFunctionValue(const std::size_t &id,
                               const double &x) noexcept {
  func &fn = funcs[id];

  if (!fn.cache.contains(x)) {
    const double val = fn.f(x);

    fn.cache.emplace(x, val);
  }

  ++fn.hits;

  return fn.cache.at(x);
}

void Cache::GetHitInfo() const {
  fprintf(stderr, "Cache hits: ");
  std::size_t i = 0;
  for (const auto &fn : funcs) {
    fprintf(stderr, "%lu - %lu, ", ++i, fn.cache.size());
  }
  fprintf(stderr, "\n");
}
