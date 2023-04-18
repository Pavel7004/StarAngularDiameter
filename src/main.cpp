#include "constants.h"
#include "theory.h"
#include <iostream>
#include <thread>

int main(void) {
  std::size_t proc_count = std::thread::hardware_concurrency();
  if (proc_count > 16) {
    proc_count = 16;
  } else {
    proc_count /= 2;
  }
  
  for (const auto &[x, y] : GetData(0, tN, proc_count)) {
    printf("%.15e %.15e\n", x, y);
  }

  return EXIT_SUCCESS;
}
