#include <iostream>
#include <cstring>

int main()
{
  std::cout << "EXPECTED: you should see an error about use-after-free on an array" << std::endl;
  auto invalid = new char[42];
  memcpy(invalid, "oops", 5);
  std::cout << invalid << std::endl;
  delete [] invalid;

#if __GNUC__ >= 12
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuse-after-free"
#endif
  // this is use-after-free and it's expected, as this test shall ensure it's detected
  *invalid = '?';
#if __GNUC__ >= 12
#pragma GCC diagnostic pop
#endif

  std::cerr << "NOTE: status code of an application should be non-zero!" << std::endl;
}
