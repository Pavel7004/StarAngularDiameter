#include "data.h"
#include <fmt/core.h>
#include <fstream>
#include <string>

datavec ReadData(const std::string& path) {
  if (path.substr(path.find('.') + 1) != "csv") {
    fmt::print(stderr, "Data file is not csv");
    std::exit(EXIT_FAILURE);
  }

  datavec res;

  std::string line;
  std::ifstream f(path);
  while (std::getline(f, line)) {
    absl::split()
  }
  f.close();

  return res;
}
