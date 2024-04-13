#pragma once

#include "inputs.h"
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

  void SendInputs(const inputs::FrameInput& inputs) noexcept;
  void ReceiveInputs(inputs::FrameInput inputs) noexcept;

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
  inputs::PlayerInput inputs_{};
  inputs::PlayerInput other_client_inputs_{};
  short current_frame_ = 0;
  short last_validated_frame_ = -1; // /!\ avec -1
  // on envoie pas les inputs des frames confirmées.


};
