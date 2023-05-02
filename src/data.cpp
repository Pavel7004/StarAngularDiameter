#include "data.h"
#include <absl/strings/str_split.h>
#include <fmt/core.h>
#include <charconv>
#include <fstream>
#include <string>
#include <string_view>

namespace {
double svtod(const std::string_view& sv) {
  double res;
  std::from_chars(sv.data(), sv.data() + sv.size(), res);
  return res;
}
};  // namespace

datavec ReadData(const std::string& path) {
  if (path.substr(path.find('.') + 1) != "csv") {
    fmt::print(stderr, "Data file is not csv. Path = {}\n", path);
    std::exit(EXIT_FAILURE);
  }

  datavec res;

  std::string line;
  std::ifstream f(path);
  while (std::getline(f, line)) {
    std::vector<std::string_view> data = absl::StrSplit(line, ',');
    if (data.size() < 2) {
      fmt::print(stderr, "Data file corrupted. Data size = {}\n", data.size());
      continue;
    }

    res.emplace_back(svtod(data[0]), svtod(data[1]));
  }
  f.close();

  return res;
}
