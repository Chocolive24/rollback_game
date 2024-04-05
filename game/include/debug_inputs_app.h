#pragma once

#include "app_interface.h"

#include <raylib.h>

#include "Vec2.h"

class DebugInputsClient {
public:
  void Init(Math::Vec2I render_tex_size) noexcept;
  void Update() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

  [[nodiscard]] const RenderTexture2D& render_texture() const noexcept {
    return render_texture_;
  }

private:
 RenderTexture2D render_texture_{};
};

class DebugInputsApp final : public Application {
public:
  void Setup() noexcept override;
  void Update() noexcept override;
  void Draw() noexcept override;
  void DrawImGui() noexcept override;
  void TearDown() noexcept override;

private:
  DebugInputsClient client_1_{};
  DebugInputsClient client_2_{};
};
