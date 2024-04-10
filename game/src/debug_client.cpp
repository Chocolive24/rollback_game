#include "debug_client.h"

#include <iostream>

#include "debug_server.h"
#include "engine.h"
#include "Random.h"

void DebugClient::Init(int client_idx,
                       Math::Vec2I render_tex_size) noexcept {
  client_idx_ = client_idx;
  render_texture_ = LoadRenderTexture(render_tex_size.X, render_tex_size.Y);
}

void DebugClient::Update() noexcept {
  inputs_ = inputs::GetPlayerInputs(client_idx_);

  if (IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    UnloadRenderTexture(render_texture_);
    render_texture_ = LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
  }

  SendInputs(inputs::FrameInputs{inputs_, current_frame_});
  current_frame_++;
}

void DebugClient::Draw() noexcept {
  BeginTextureMode(render_texture_);
  {
    ClearBackground(BLACK);

    const auto tex_center = Math::Vec2I(render_texture_.texture.width / 2,
                                        render_texture_.texture.height / 2);
    const auto client_1_txt_pos_x = tex_center.X - tex_center.X / 2;
    const auto client_2_txt_pos_x = tex_center.X + tex_center.X / 2;

    const char* client_idx_txt = TextFormat("Client %i", client_idx_);
    DrawText(client_idx_txt, tex_center.X - MeasureText(client_idx_txt, kFontSize) / 2, 
        75.f, kFontSize, WHITE);

    DrawText("Client 1",
             client_1_txt_pos_x -  MeasureText(client_idx_txt, kFontSize) / 2,
             175.f, kFontSize, WHITE);
    DrawText("Client 2",
             client_2_txt_pos_x - MeasureText(client_idx_txt, kFontSize) / 2,
             175.f, kFontSize, WHITE);

    for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
      int text_pos_x = i == 0 ? client_1_txt_pos_x : client_2_txt_pos_x;

      if (i == 0)
        text_pos_x = client_idx_ == 1 ? client_1_txt_pos_x : client_2_txt_pos_x;
      else
        text_pos_x = client_idx_ == 1 ? client_2_txt_pos_x : client_1_txt_pos_x;

      const inputs::PlayerInputs inputs = i == 0 ? inputs_ : other_client_inputs_;
      

      Color key_color = inputs & static_cast<inputs::PlayerInputs>(
                                      inputs::PlayerInputTypes::kUp)
                            ? WHITE
                            : DARKGRAY;
      DrawText("UP", text_pos_x - MeasureText("UP", kFontSize) / 2,
               kStartTextPosY + 1 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInputs>(
                                inputs::PlayerInputTypes::kLeft)
                      ? WHITE
                      : DARKGRAY;
      DrawText("LEFT", text_pos_x - MeasureText("LEFT", kFontSize) / 2,
               kStartTextPosY + 2 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInputs>(
                                inputs::PlayerInputTypes::kDown)
                      ? WHITE
                      : DARKGRAY;
      DrawText("DOWN", text_pos_x - MeasureText("DOWN", kFontSize) / 2,
               kStartTextPosY + 3 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInputs>(
                                inputs::PlayerInputTypes::kRight)
                      ? WHITE
                      : DARKGRAY;
      DrawText("RIGHT", text_pos_x - MeasureText("RIGHT", kFontSize) / 2,
               kStartTextPosY + 4 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInputs>(
                               inputs::PlayerInputTypes::kShoot)
                      ? WHITE
                      : DARKGRAY;

      DrawText("SHOOT", text_pos_x - MeasureText("SHOOT", kFontSize) / 2,
               kStartTextPosY + 5 * kTextOffsetY, kFontSize, key_color);
    }
  }
  EndTextureMode();
}

void DebugClient::Deinit() noexcept {
  UnloadRenderTexture(render_texture_); }

void DebugClient::SendInputs(inputs::FrameInputs inputs) noexcept {
  const auto delay = Math::Random::Range(server_->min_packet_delay, 
                                              server_->max_packet_delay);
  const inputs::DebugInputs debug_inputs{inputs, delay, client_idx_};
  if (inputs.inputs != 0)
  {
    std::cout << "client " << client_idx_ << " sent inputs from frame nbr " << 
   " " << inputs.frame_nbr << '\n';
  }
  server_->ReceiveInputs(debug_inputs);
}

void DebugClient::ReceiveInputs(inputs::FrameInputs inputs) noexcept {
  other_client_inputs_ = inputs.inputs;

  if (inputs.inputs != 0)
  {
    std::cout << "client " << client_idx_ << " received inputs from frame nbr " << 
    " " << inputs.frame_nbr << '\n';
  }
}