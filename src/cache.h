#ifndef __STAR_CACHE__
#define __STAR_CACHE__

#include <absl/container/flat_hash_map.h>
#include <functional>
#include <utility>
#include <vector>

class Cache {
  using val_func = std::function<double(const double&)>;
  using map = absl::flat_hash_map<double, double>;

  struct func {
    map cache;
    val_func f;

    explicit func(val_func f) noexcept : cache(), f(std::move(f)) {
      cache.reserve(4'500'000);
    }

    ~func() noexcept = default;

    func(const func&) = default;
    func(func&&) = default;

    func& operator=(const func&) noexcept = default;
    func& operator=(func&&) noexcept = default;
  };

 public:
  explicit Cache() noexcept = default;
  ~Cache() noexcept = default;

  Cache(const Cache&) = delete;
  explicit Cache(Cache&& cache) noexcept = default;

  Cache& operator=(const Cache&) = delete;
  Cache& operator=(Cache&&) = default;

  std::size_t RegisterFunction(const val_func& f) noexcept;

  double GetFunctionValue(const std::size_t& id, const double& x) noexcept;

 private:
  std::vector<func> funcs;
};

#endif  // __STAR_CACHE__
