#include "client.h"

#include <imgui.h>

#include <string>

#include "engine.h"

void Client::Init(int input_profile_id) noexcept {
  input_profile_id_ = input_profile_id;

  game_renderer_.Init();

  audio_manager_.Init();
  audio_manager_.PlayMusic(MusicType::kStartMenu);
}

void Client::Update() noexcept {
  const auto delta_time = raylib::GetFrameTime();
  fixed_timer_ += delta_time;
  time_since_last_fixed_update_ += delta_time;

  audio_manager_.Update();

  while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
    if (state_ == ClientState::kInGame) {
      online_game_manager_.FixedUpdateCurrentFrame();
    }

    fixed_timer_ -= game_constants::kFixedDeltaTime;
    time_since_last_fixed_update_ = 0.f;
  }
}

void Client::Draw(const raylib::RenderTexture2D& render_texture,
                  raylib::Vector2 render_target_pos) noexcept {
  raylib::ClearBackground(raylib::BLACK);

  switch (state_) {
    case ClientState::kInGame:
      game_renderer_.Draw(render_texture, render_target_pos,
                          time_since_last_fixed_update_);
    break;
    case ClientState::kConnecting:
    case ClientState::kInMainMenu:
    case ClientState::kInRoom:
      raylib::BeginTextureMode(render_texture);
      DrawMainMenu();
      raylib::EndTextureMode();
    break;
    default:
      break;
  }
}

void Client::DrawImGui() noexcept {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  // Assuming window_size is already defined
  const auto window_size = Engine::window_size();

  // Calculate the center of the window
  float centerX = (window_size.X * 0.5f - 300 * 0.5f);
  float centerY = (window_size.Y * 0.5f - 200 * 0.5f);

  // Set the ImGui window position to the center
  //ImGui::SetNextWindowPos(ImVec2(centerX, centerY));

  const std::string window_name = "Menu";

  const auto flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                     ImGuiWindowFlags_NoCollapse;

  switch (state_) {
    case ClientState::kConnecting:
      ImGui::Begin(window_name.c_str(), nullptr, flags);
      ImGui::TextWrapped("Connecting...");
      ImGui::End();
      break;
    case ClientState::kInRoom:
      ImGui::Begin(window_name.c_str(), nullptr, flags);
      ImGui::TextWrapped("Waiting for an other player...");
      ImGui::End();
      break;
    case ClientState::kInMainMenu: {
      ImGui::Begin(window_name.c_str(), nullptr, flags);
      {
        if (ImGui::Button("Join a Game", ImVec2(125, 25))) {
          network_interface_->JoinRandomOrCreateRoom();
        }

        ImGui::Spacing();
        ImGui::Checkbox("Play Sound", &audio_manager_.is_audio_enabled);

        ImGui::Spacing();
        float global_volume = audio_manager_.global_volume();
        ImGui::SliderFloat("Volume", &global_volume, 0.0f, 1.f);
        audio_manager_.SetGlobalVolume(global_volume);

        ImGui::Spacing();
        if (ImGui::Button("Quite", ImVec2(125, 25))) {
          raylib::CloseWindow();
        }
      }
      ImGui::End();
      break;
    }

    case ClientState::kInGame:
      if (online_game_manager_.is_finished()) {
        ImGui::Begin(window_name.c_str(), nullptr, flags);
        {
          if (ImGui::Button("Go back to Menu", ImVec2(125, 25))) {
            online_game_manager_.Deinit();
            state_ = ClientState::kInMainMenu;
            network_interface_->LeaveRoom();
            audio_manager_.PlayMusic(MusicType::kStartMenu);
          }
          ImGui::Spacing();

          if (ImGui::Button("Quite", ImVec2(125, 25))) {
            raylib::CloseWindow();
          }
        }

        ImGui::End();
        break;
      }
    default:
      break;
  }
}

void Client::Deinit() noexcept {
  online_game_manager_.Deinit();
  game_renderer_.Deinit();
}

void Client::OnNetworkEventReceived(
    const NetworkEvent& network_event) noexcept {
  online_game_manager_.PushNetworkEvent(network_event);
}

void Client::StartGame() noexcept {
  state_ = ClientState::kInGame;

  // PlayerId is in range 0-1 but ClientId is in range 1-2.
  online_game_manager_.SetPlayerId(client_id_ - 1);
  online_game_manager_.Init(input_profile_id_);

  audio_manager_.PlayMusic(MusicType::kBattle);
}

void Client::DrawMainMenu() {
  static auto ice_ground = CreateSprite("data/images/ice.png", {2.f, 2.f});

  const auto window_size = Engine::window_size();

  for (int y = 0; y < window_size.Y; y += 36) {
    for (int x = 0; x < window_size.X + 64; x += 64) {
      ice_ground.Draw(
          raylib::Vector2{static_cast<float>(x), static_cast<float>(y)});
    }
  }

  static Sprite photon_logo =
      CreateSprite("data/images/by-photon_short_b.png", {1.f, 1.f});
  photon_logo.Draw({window_size.X * 0.5f, window_size.Y * 0.15f});

  static Sprite raylib_logo =
      CreateSprite("data/images/raylib_logo.png", {0.5f, 0.5f});
  raylib_logo.Draw({window_size.X * 0.5f, window_size.Y * 0.85f});

  static raylib::Texture2D blue_spin_animation =
      raylib::LoadTexture("data/images/blue_penguin/SpinAttack.png");
  static raylib::Texture2D red_spin_animation =
      raylib::LoadTexture("data/images/red_penguin/SpinAttack.png");
  static constexpr std::int8_t kSpinAnimFrameCount = 7;
  static constexpr float kSpinAnimFrameRate = 12.f;
  static raylib::Rectangle spin_anim_rec{};
  static float spin_anim_frame_counter = 0;

  static bool init = false;
  if (!init) {
    blue_spin_animation.width *= 10;
    blue_spin_animation.height *= 10;
    red_spin_animation.width *= 10;
    red_spin_animation.height *= 10;

    init = true;
  }
  SetTextureWrap(blue_spin_animation, raylib::TEXTURE_WRAP_CLAMP);
  SetTextureWrap(red_spin_animation, raylib::TEXTURE_WRAP_CLAMP);

  spin_anim_rec = {
      0.0f, 0.0f,
      static_cast<float>(blue_spin_animation.width) / kSpinAnimFrameCount,
      static_cast<float>(blue_spin_animation.height)};

  // Update animation frame counter
  spin_anim_frame_counter += raylib::GetFrameTime() * kSpinAnimFrameRate;

  // Check if it's time to advance to the next frame
  if (spin_anim_frame_counter > kSpinAnimFrameCount) {
    spin_anim_frame_counter -= kSpinAnimFrameCount;
  }

  // Calculate the current frame based on the animation frame counter
  const int current_frame = static_cast<int>(spin_anim_frame_counter);

  // Update the x position of the source rectangle for the current frame
  spin_anim_rec.x = static_cast<float>(current_frame) *
                    static_cast<float>(blue_spin_animation.width) /
                    kSpinAnimFrameCount;

  raylib::Rectangle source = {spin_anim_rec.x, spin_anim_rec.y,
                              spin_anim_rec.width, spin_anim_rec.height};

  // Draw the current frame of the animation
  DrawTextureRec(
      blue_spin_animation, source,
      raylib::Vector2{225 - spin_anim_rec.width / 2,
                      window_size.Y / 2 - spin_anim_rec.height / 2.5f},
      raylib::WHITE);

  source = {spin_anim_rec.x, spin_anim_rec.y, -spin_anim_rec.width,
            spin_anim_rec.height};
  DrawTextureRec(
      red_spin_animation, source,
      raylib::Vector2{window_size.X - 225 - spin_anim_rec.width / 2,
                      window_size.Y / 2 - spin_anim_rec.height / 2.5f},
      raylib::WHITE);
}
