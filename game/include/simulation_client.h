#pragma once

#include "network_interface.h"
#include "rollback_manager.h"
#include "game_renderer.h"

#include <vector>

#include "Timer.h"

struct FrameToConfirm {
  int check_sum = 0;
  FrameNbr frame_nbr = 0;
  float delay = 0.f;
};

/**
 * \brief SimulationClient is a class that simulates the game on the client side,
 * with a mock of the network interface that simulates the network.
 *
 * This allows you to test the game locally, controlling
 * network delays and packet loss.
 */
class SimulationClient final : public NetworkInterface {
 public:
  void Init(int input_profile_id, PlayerId player_id) noexcept;
  void RegisterOtherClient(SimulationClient* other_client) noexcept;
  void Update() noexcept;
  void FixedUpdate() noexcept;
  void Draw(const raylib::RenderTexture2D& render_target) noexcept;
  void Deinit() noexcept;

  void RaiseEvent(bool reliable, EventCode event_code,
                  const ExitGames::Common::Hashtable& event_data) noexcept override;
  void ReceiveEvent(int player_nr, EventCode event_code,
                    const ExitGames::Common::Hashtable& event_content) noexcept override;

  [[nodiscard]] PlayerId player_id() const noexcept {
    return player_id_;
  }

  static float min_packet_delay;
  static float max_packet_delay;
  static float packet_loss_percentage;

private:
  void SendInputEvent();
  void PollInputPackets();
  void PollConfirmFramePackets();

  GameManager game_manager_{};
  GameRenderer game_renderer_{&game_manager_};
  RollbackManager rollback_manager_{};

  std::vector<inputs::PlayerInput> inputs_{};
  std::vector<FrameNbr> frames_{};

  SimulationClient* other_client_ = nullptr;
  std::vector<inputs::SimulationInput> waiting_input_queue{};
  std::vector<FrameToConfirm> waiting_frame_queue_{};

  Timer timer_{};
  float fixed_timer_ = game_constants::kFixedDeltaTime;

  static constexpr PlayerId kMasterClientId = 0;
  static constexpr int kBaseInputSize = 1000;
  static constexpr int kStartTextPosY = 200;
  static constexpr int kTextOffsetY = 75;
  static constexpr int kFontSize = 35;

  PlayerId player_id_ = -1;
  int input_profile_id_ = -1;
  std::int16_t current_frame_ = -1;
};
