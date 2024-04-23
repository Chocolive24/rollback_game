#pragma once

#include "network_interface.h"
#include "rollback_manager.h"
#include "game_renderer.h"
#include "network_game_manager.h"

#include <vector>

struct SimulationFrameToConfirm {
  int check_sum = 0;
  std::vector<inputs::FrameInput> frame_inputs{};
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

  static float min_packet_delay;
  static float max_packet_delay;
  static float packet_loss_percentage;

private:
  void PollInputPackets();
  void PollConfirmFramePackets();

  RollbackManager rollback_manager_{};
  NetworkGameManager network_game_manager_{};
  GameRenderer game_renderer_{&network_game_manager_};

  SimulationClient* other_client_ = nullptr;
  std::vector<inputs::SimulationInput> waiting_input_queue{};
  std::vector<SimulationFrameToConfirm> waiting_frame_queue_{};

  float fixed_timer_ = game_constants::kFixedDeltaTime;

  static constexpr PlayerId kMasterClientId = 0;
  static constexpr int kBaseInputSize = 100;
};
