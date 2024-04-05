#include "client_application.h"
#include "engine.h"

int main() {
  ClientApplication client_application{};
  Engine engine(&client_application);

  engine.Run();

  return EXIT_SUCCESS;
}