#pragma once

#include <vector>

#include "game_renderer.h"
#include "inputs.h"
#include "network_interface.h"
#include "rollback_manager.h"

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
  GameManager game_manager_{};
  GameRenderer game_renderer_{&game_manager_};
  RollbackManager rollback_manager_{};

  SimulationClient* other_client_ = nullptr;

  //std::vector<inputs::SimulationInput> local_inputs_{};
  //std::vector<inputs::SimulationInput> other_client_inputs_{};
  std::vector<inputs::SimulationInput> waiting_input_queue{};

  static constexpr int kBaseInputSize = 1000;
  static constexpr int kStartTextPosY = 200;
  static constexpr int kTextOffsetY = 75;
  static constexpr int kFontSize = 35;

  PlayerId player_id_ = -1;
  int input_profile_id_ = -1;
  std::int16_t current_frame_ = -1;

  // /!\ avec -1
  // on envoie pas les inputs des frames confirmées.
};
