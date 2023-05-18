#include "cache.h"

Cache::Func::Func(val_func f) noexcept : f(std::move(f)) {
  cache.reserve(4'500'000);
}

std::size_t Cache::RegisterFunction(const val_func& f) noexcept {
  funcs_.emplace_back(f);
  return funcs_.size() - 1;
}

[[nodiscard]] double Cache::GetFunctionValue(const std::size_t& id,
                                             const double& x) const noexcept {
  const auto& fn = funcs_.at(id);

  if (!fn.cache.contains(x)) {
    const double val = fn.f(x);

    fn.cache.emplace(x, val);

    return val;
  }

  return fn.cache.at(x);
}
