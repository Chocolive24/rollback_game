#pragma once

#include "application.h"
#include "client.h"

class ClientApplication final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  Client client_{};
  raylib::RenderTexture2D render_texture_{};
};
