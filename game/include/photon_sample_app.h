#pragma once
#include "application.h"
#include "network_logic.h"

class PhotonSampleApp final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  ExitGames::Common::JString appID =
      L"094ac400-9d70-4877-97b1-6ed7c0b31c4d";  // set your app id here
  ExitGames::Common::JString appVersion = L"1.0";
  SampleNetworkLogic networkLogic_{appID, appVersion};
};
