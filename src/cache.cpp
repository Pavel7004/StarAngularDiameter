#include "cache.h"

std::size_t Cache::RegisterFunction(const val_func& f) noexcept {
  funcs_.emplace_back(f);
  return funcs_.size() - 1;
}

double Cache::GetFunctionValue(const std::size_t& id,
                               const double& x) noexcept {
  Func& fn = funcs_[id];

  if (!fn.cache.contains(x)) {
    const double val = fn.f(x);

    fn.cache.emplace(x, val);
  }

  return fn.cache.at(x);
}