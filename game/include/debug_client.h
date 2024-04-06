#pragma once

#include "inputs_manager.h"
#include "Vec2.h"

#include <raylib.h>

class DebugInputsClient {
 public:
  void Init(int client_idx, Math::Vec2I render_tex_size) noexcept;
  void Update() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

  void SendPacket(game::PlayerInputs inputs,
                  DebugInputsClient* receiver) noexcept;
  game::PlayerInputs ReceivePacket(game::PlayerInputs inputs) noexcept;

  [[nodiscard]] const RenderTexture2D& render_texture() const noexcept {
    return render_texture_;
  }

 private:
  RenderTexture2D render_texture_{};
  int client_idx_ = 0;
  static constexpr int kTextOffsetY = 75;
  game::PlayerInputs inputs_{};
  game::PlayerInputs other_client_inputs_{};
};
