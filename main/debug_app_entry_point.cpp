#include "debug_app.h"
#include "engine.h"

int main() {
  DebugApp debug_inputs_app{};
  Engine engine(&debug_inputs_app);

  engine.Run();

  return EXIT_SUCCESS;
}