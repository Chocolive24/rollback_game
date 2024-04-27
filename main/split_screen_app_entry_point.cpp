#include "engine.h"
#include "split_screen_app.h"

int main() {
  SplitScreenApp app{};
  Engine engine{&app};

  engine.Run();

  return EXIT_SUCCESS;
}