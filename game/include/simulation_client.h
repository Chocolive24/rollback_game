#pragma once

#include "network_interface.h"
#include "game_renderer.h"
#include "online_game_manager.h"

#include <vector>

/**
 * \brief SimulationInput is a struct containing a frame input and have a delay
 * value to simulate the network delay.
 */
struct SimulationInput {
  // std::vector<FrameInput> frame_inputs;
  std::vector<input::FrameInput> frame_inputs;
  float delay = 0.f;
};

struct SimulationFrameToConfirm {
  int check_sum = 0;
  std::vector<input::FrameInput> frame_inputs{};
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
  void Draw(const raylib::RenderTexture2D& render_target, raylib::Vector2 render_target_pos) noexcept;
  void Deinit() noexcept;

  void RaiseEvent(bool reliable, NetworkEventCode event_code,
                  const ExitGames::Common::Hashtable& event_data) noexcept override;
  void ReceiveEvent(int player_nr, NetworkEventCode event_code,
                    const ExitGames::Common::Hashtable& event_content) noexcept override;

  static float min_packet_delay;
  static float max_packet_delay;
  static float packet_loss_percentage;

private:
  void PollInputPackets();
  void PollConfirmFramePackets();
  OnlineGameManager online_game_manager_{};
  GameRenderer game_renderer_{&online_game_manager_};

  SimulationClient* other_client_ = nullptr;
  std::vector<SimulationInput> waiting_input_queue{};
  std::vector<SimulationFrameToConfirm> waiting_frame_queue_{};

  float fixed_timer_ = game_constants::kFixedDeltaTime;
  float time_since_last_fixed_update_ = 0.f;
  static constexpr PlayerId kMasterClientId = 0;
  static constexpr int kBaseInputSize = 100;
};
