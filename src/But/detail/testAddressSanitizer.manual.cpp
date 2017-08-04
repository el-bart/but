#include <iostream>
#include <cstring>

int main()
{
  std::cout << "EXPECTED: you should see an error about use-after-free on an array" << std::endl;
  auto invalid = new char[42];
  memcpy(invalid, "oops", 5);
  std::cout << invalid << std::endl;
  delete [] invalid;
  *invalid = '?';
  std::cerr << "NOTE: status code of an application should be non-zero!" << std::endl;
}
