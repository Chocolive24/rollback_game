#pragma once

#include "inputs_manager.h"
#include "Vec2.h"

#include <raylib.h>

class DebugServer;

class DebugClient {
 public:
  void RegisterServer(DebugServer* server) noexcept { server_ = server; }
  void Init(int client_idx, Math::Vec2I render_tex_size) noexcept;
  void Update() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

  void SendInputs(inputs::FrameInputs inputs) noexcept;
  void ReceiveInputs(inputs::FrameInputs inputs) noexcept;

  [[nodiscard]] const RenderTexture2D& render_texture() const noexcept {
    return render_texture_;
  }

 private:
  DebugServer* server_ = nullptr;
  RenderTexture2D render_texture_{};
  int client_idx_ = 0;
  static constexpr int kStartTextPosY = 200;
  static constexpr int kTextOffsetY = 75;
  static constexpr int kFontSize = 35;
  inputs::PlayerInputs inputs_{};
  inputs::PlayerInputs other_client_inputs_{};
  std::uint32_t current_frame_ = 0;
  std::uint32_t last_validated_frame_ = 0;
};
