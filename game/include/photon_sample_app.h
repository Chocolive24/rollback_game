#pragma once

#include "application.h"
#include "network_manager.h"

class PhotonSampleApp final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  ExitGames::Common::JString app_id_ = PHOTON_APP_ID;
  ExitGames::Common::JString app_version_ = L"1.0";
  NetworkManager networkLogic_{app_id_, app_version_};
};