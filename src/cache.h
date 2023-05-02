#ifndef STAR_CACHE_
#define STAR_CACHE_

#include <absl/container/flat_hash_map.h>
#include <functional>
#include <vector>

class Cache {
  using val_func = std::function<double(const double&)>;
  using map = absl::flat_hash_map<double, double>;

  struct Func {
    mutable map cache;
    val_func f;

    explicit Func(val_func f) noexcept;

    ~Func() noexcept = default;

    Func(const Func&) = default;
    Func(Func&&) = default;

    Func& operator=(const Func&) noexcept = default;
    Func& operator=(Func&&) noexcept = default;
  };

 public:
  explicit Cache() noexcept = default;
  ~Cache() noexcept = default;

  Cache(const Cache&) = delete;
  Cache(Cache&& cache) noexcept = default;

  Cache& operator=(const Cache&) = delete;
  Cache& operator=(Cache&&) noexcept = default;

  std::size_t RegisterFunction(const val_func& f) noexcept;

  [[nodiscard]] double GetFunctionValue(const std::size_t& id,
                                        const double& x) const noexcept;

 private:
  std::vector<Func> funcs_;
};

#endif  // STAR_CACHE_
