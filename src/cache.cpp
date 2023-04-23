#include "cache.h"
#include <utility>

std::size_t Cache::RegisterFunction(const val_func &f) noexcept {
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

  return fn.cache.at(x);
}
