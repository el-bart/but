#include <iostream>

int main()
{
  std::cout << "EXPECTED: you should see an error about memory leak" << std::endl;
  auto leak = new char[42];
  (void)leak;
  std::cerr << "NOTE: status code of an application should be non-zero!" << std::endl;
}
