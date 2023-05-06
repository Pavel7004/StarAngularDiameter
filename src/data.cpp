#include "data.h"

#include <absl/strings/str_split.h>
#include <fmt/core.h>
#include <charconv>
#include <fstream>
#include <string>
#include <string_view>

#include "datavec.h"

namespace {
double svtod(const absl::string_view& sv) {
  double res;
  std::from_chars(sv.data(), sv.data() + sv.size(), res);
  return res;
}
};  // namespace

DataArray ReadData(const std::string& path) {
  if (path.substr(path.find('.') + 1) != "csv") {
    fmt::print(stderr, "Data file is not csv. Path = {}\n", path);
    std::exit(EXIT_FAILURE);
  }

  std::vector<double> res_t;
  std::vector<double> res_data;

  std::string line;
  std::ifstream f(path);
  while (std::getline(f, line)) {
    std::vector<absl::string_view> data = absl::StrSplit(line, ',');
    if (data.size() < 2) {
      fmt::print(stderr, "Data file corrupted. Data size = {}\n", data.size());
      continue;
    }

    res_t.emplace_back((svtod(data[0]) * 1e-3));
    res_data.emplace_back(svtod(data[1]));
  }
  f.close();

  return {
      .t = res_t,
      .x = {},
      .N_model = {},
      .N_data = res_data,
  };
}
