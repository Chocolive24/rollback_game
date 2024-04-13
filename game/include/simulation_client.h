#pragma once

#include <vector>

#include "inputs.h"
#include "network_interface.h"
#include "raylib_wrapper.h"
#include "Vec2.h"

class SimulationClient final : public NetworkInterface {
 public:
  void Init(int local_player_id, 
      Math::Vec2I render_tex_size) noexcept;
  void RegisterOtherClient(SimulationClient* other_client) noexcept;
  void Update() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;

  void RaiseEvent(bool reliable, EventCode event_code,
                  const ExitGames::Common::Hashtable& event_data) noexcept override;
  void ReceiveEvent(int player_nr, EventCode event_code,
                    const ExitGames::Common::Hashtable& event_content) noexcept override;

  [[nodiscard]] const raylib::RenderTexture2D& render_texture() const noexcept {
    return render_texture_;
  }

  static float min_packet_delay;
  static float max_packet_delay;
  static float packet_loss_percentage;

private:
  SimulationClient* other_client_ = nullptr;
  raylib::RenderTexture2D render_texture_{};
  std::vector<inputs::SimulationInput> local_inputs_{};
  std::vector<inputs::SimulationInput> other_client_inputs_{};
  static constexpr int kBaseInputSize = 1000;
  static constexpr int kStartTextPosY = 200;
  static constexpr int kTextOffsetY = 75;
  static constexpr int kFontSize = 35;
  int local_player_id_ = -1;
  std::int16_t current_frame_ = -1;

  // /!\ avec -1
  // on envoie pas les inputs des frames confirmées.
};
