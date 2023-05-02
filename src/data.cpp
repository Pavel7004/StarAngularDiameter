#include "data.h"
#include <fmt/core.h>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

datavec ReadData(const std::string& path) {
  if (path.substr(path.find('.') + 1) != "csv") {
    fmt::print(stderr, "Data file is not csv");
    exit(EXIT_FAILURE);
  }

  datavec res;

  std::string line;
  std::ifstream f(path);
  while (std::getline(f, line)) {
    std::istringstream parse(line);
    double x;
    double y;

    parse >> x >> y;

    res.emplace_back(x, y);
  }
  f.close();

  return res;
}
