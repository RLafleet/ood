#include "Simulation.h"

#include <iostream>

int main(const int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <single|parallel>" << std::endl;
    return 1;
  }

  const bool multiThreaded = (std::string(argv[1]) == "parallel");

  Simulation simulation(multiThreaded);

  simulation.Start();

  return 0;
}