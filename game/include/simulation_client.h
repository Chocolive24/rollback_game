#pragma once

#include "inputs.h"
#include "network_interface.h"
#include "raylib_wrapper.h"
#include "Vec2.h"

class SimulationClient final : public NetworkInterface {
 public:
  void Init(int client_idx, Math::Vec2I render_tex_size) noexcept;
  void Update() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

  void RaiseEvent(bool reliable, const ExitGames::Common::Hashtable& data,
                  EventCode event_code) noexcept override;
  void ReceiveEvent(
      int player_nr, EventCode event_code,
      const ExitGames::Common::Object& event_content) noexcept override;

  //void SendInputs(const inputs::FrameInput& inputs) noexcept;
  //void ReceiveInputs(inputs::FrameInput inputs) noexcept;

  [[nodiscard]] const raylib::RenderTexture2D& render_texture() const noexcept {
    return render_texture_;
  }



private:
  raylib::RenderTexture2D render_texture_{};
  int client_idx_ = 0;
  static constexpr int kStartTextPosY = 200;
  static constexpr int kTextOffsetY = 75;
  static constexpr int kFontSize = 35;
  inputs::PlayerInput inputs_{};
  inputs::PlayerInput other_client_inputs_{};
  std::int16_t current_frame_ = 0;
  std::int16_t last_validated_frame_ = -1;

  // /!\ avec -1
  // on envoie pas les inputs des frames confirmées.
};
