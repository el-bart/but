#include <thread>
#include <iostream>

int main()
{
  std::cout << "EXPECTED: you should see an error about data race on variable being modified from multiple threads" << std::endl;
  volatile int counter = 0;
  std::thread th1{ [&]() { for(auto i=0; i<1000*1000; ++i) ++counter; } };
  std::thread th2{ [&]() { for(auto i=0; i<1000*1000; ++i) ++counter; } };
  th1.join();
  th2.join();
  std::cerr << "NOTE: status code of an application should be non-zero!" << std::endl;
}
