#pragma once

#include "app_interface.h"

#include <raylib.h>

class DebugInputsApp final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  RenderTexture2D client_1_tex_{};
  RenderTexture2D client_2_tex_{};
};
