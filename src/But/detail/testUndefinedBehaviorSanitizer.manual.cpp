#include <limits>
#include <iostream>

int main()
{
  std::cout << "EXPECTED: you should see an error about overflowing a signed integer" << std::endl;
  auto overflow = std::numeric_limits<signed>::max();
  ++overflow;
  (void)overflow;
  std::cerr << "NOTE: status code of an application should be non-zero!" << std::endl;
}
