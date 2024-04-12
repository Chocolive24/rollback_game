#include "engine.h"
#include "photon_sample_app.h"

int main() {
  PhotonSampleApp app{};
  Engine engine(&app);
  engine.Run();

  return EXIT_SUCCESS;
}
