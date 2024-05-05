#pragma once

#include "network_manager.h"
#include "game_renderer.h"
#include "online_game_manager.h"
#include "audio_manager.h"
#include "types.h"

enum class ClientState {
  kConnecting,
  kInMainMenu,
  kInRoom,
  kInGame
};

/**
 * \brief Client is a class that has a network manager and an online game manager
 * to coordinate network code and game logic code.
 *
 * It also has a game renderer to draw the game in a render target given by an application.
 */
class Client {
 public:
  void Init(int input_profile_id) noexcept;
  void Update() noexcept;
  void Draw(const raylib::RenderTexture2D& render_texture, raylib::Vector2 render_target_pos) noexcept;
  void DrawImGui() noexcept;
  void Deinit() noexcept;

  void StartGame() noexcept;

  void SetState(const ClientState state) noexcept { state_ = state; }

  [[nodiscard]] ClientId client_id() const noexcept { return client_id_; }
  void SetClientId(const ClientId client_id) noexcept {
    client_id_ = client_id;
  }

private:
  NetworkManager network_manager_{PHOTON_APP_ID, L"1.0"};
  OnlineGameManager online_game_manager_{};

  GameRenderer game_renderer_{&online_game_manager_};

  AudioManager audio_manager_{};

  float fixed_timer_ = game_constants::kFixedDeltaTime;
  float time_since_last_fixed_update_ = 0.f;
  ClientState state_ = ClientState::kConnecting;

  ClientId client_id_ = game_constants::kInvalidClientId;
};
