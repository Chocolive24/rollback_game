#include "engine.h"
#include "photon_sample_app.h"

int main() {
  //static const ExitGames::Common::JString appID =
  //    L"094ac400-9d70-4877-97b1-6ed7c0b31c4d";  // set your app id here
  //static const ExitGames::Common::JString appVersion = L"1.0";

  //SampleNetworkLogic networkLogic(appID, appVersion);

  //networkLogic.connect();

  PhotonSampleApp app{};
  Engine engine(&app);
  engine.Run();

  return EXIT_SUCCESS;

  //while (!shouldExit) {
  //  networkLogic.run();
  //  SLEEP(100);
  //}
}
