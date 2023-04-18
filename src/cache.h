#ifndef __STAR_CACHE__
#define __STAR_CACHE__

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class Cache {
  using val_func = std::function<double(const double &)>;

  struct func {
    std::unordered_map<double, double> cache;
    std::size_t hits;
    val_func f;

    explicit func(const val_func &f) : cache(), hits(0), f(f) {
      cache.reserve(4'500'000);
    }
    ~func() = default;
  };

public:
  explicit Cache() = default;
  ~Cache() = default;

  std::size_t RegisterFunction(const val_func &f) noexcept;

  double GetFunctionValue(const std::size_t &id, const double &x) noexcept;

  void GetHitInfo() const;

private:
  std::vector<func> funcs;
};

#endif // __STAR_CACHE__
