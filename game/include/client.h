#pragma once
#include "network_interface.h"
#include "online_game_manager.h"
#include "audio_manager.h"
#include "game_renderer.h"
#include "types.h"

enum class ClientState { kConnecting, kInMainMenu, kInRoom, kInGame };

/**
 * \brief Client is a class that has a network manager and an online game
 * manager to coordinate network code and game logic code.
 *
 * It also has a game renderer to draw the game in a render target given by an
 * application.
 */
class Client {
 public:
  void RegisterNetworkInterface(NetworkInterface* network_interface) noexcept {
    network_interface_ = network_interface;
    online_game_manager_.RegisterNetworkInterface(network_interface);
  }
  void Init(int input_profile_id) noexcept;
  void Update() noexcept;
  void DrawMainMenu();
  void Draw(const raylib::RenderTexture2D& render_texture,
            raylib::Vector2 render_target_pos) noexcept;
  void DrawImGui() noexcept;
  void Deinit() noexcept;

  void OnNetworkEventReceived(const NetworkEvent& network_event) noexcept;

  void StartGame() noexcept;

  void SetState(const ClientState state) noexcept { state_ = state; }

  [[nodiscard]] bool is_in_game() const noexcept {
    return state_ == ClientState::kInGame;
  }

  [[nodiscard]] ClientId client_id() const noexcept { return client_id_; }
  void SetClientId(const ClientId client_id) noexcept {
    client_id_ = client_id;
  }

 private:
  NetworkInterface* network_interface_ = nullptr;
  OnlineGameManager online_game_manager_{};

  GameRenderer game_renderer_{&online_game_manager_};

  AudioManager audio_manager_{};

  float fixed_timer_ = game_constants::kFixedDeltaTime;
  float time_since_last_fixed_update_ = 0.f;
  ClientState state_ = ClientState::kConnecting;

  ClientId client_id_ = game_constants::kInvalidClientId;
  int input_profile_id_ = 0;

  // Main menu attributes.
  // =====================

  Sprite ice_ground{};
  Sprite photon_logo{};
  Sprite raylib_logo{};
  raylib::Texture2D blue_spin_animation{};
  raylib::Texture2D red_spin_animation{};
  static constexpr std::int8_t kSpinAnimFrameCount = 7;
  static constexpr float kSpinAnimFrameRate = 12.f;
  raylib::Rectangle spin_anim_rec{};
  float spin_anim_frame_counter = 0;
};
