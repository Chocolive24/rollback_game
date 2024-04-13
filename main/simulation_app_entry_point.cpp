#include "simulation_app.h"
#include "engine.h"

int main() {
  SimulationApp simulation_app{};
  Engine engine(&simulation_app);

  engine.Run();

  return EXIT_SUCCESS;
}