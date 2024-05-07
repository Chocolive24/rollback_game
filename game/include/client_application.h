#pragma once

#include "application.h"
#include "network_manager.h"
#include "client.h"


class ClientApplication final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  NetworkManager network_manager_{PHOTON_APP_ID, L"1.0"};
  Client client_{};
  raylib::RenderTexture2D render_texture_{};
};
