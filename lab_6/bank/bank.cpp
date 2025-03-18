#include "Simulation.h"
std::atomic<bool> Simulation::simulation_running_{true};

int main(int argc, char* argv[]) {
  std::signal(SIGINT, Simulation::SignalHandler);
  std::signal(SIGTERM, Simulation::SignalHandler);

  Simulation simulation;
  const bool multithreaded = (argc > 1 && std::string(argv[1]) == "parallel");
  simulation.Run(multithreaded);

  return 0;
}