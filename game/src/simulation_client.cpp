#include "simulation_client.h"
#include "engine.h"
#include "game_constants.h"
#include "Random.h"

#include <iostream>

void SimulationClient::Init(int client_idx,
                       Math::Vec2I render_tex_size) noexcept {
  client_idx_ = client_idx;
  render_texture_ = raylib::LoadRenderTexture(render_tex_size.X, render_tex_size.Y);
}

void SimulationClient::Update() noexcept {
  inputs_ = inputs::GetPlayerInput(client_idx_);

  if (raylib::IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    UnloadRenderTexture(render_texture_);
    render_texture_ = raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
  }

  //SendInputs(inputs::FrameInput{inputs_, current_frame_});
  current_frame_++;
}

void SimulationClient::Draw() noexcept {
  raylib::BeginTextureMode(render_texture_);
  {
    raylib::ClearBackground(raylib::kBlack);

    const auto tex_center = Math::Vec2I(render_texture_.texture.width / 2,
                                        render_texture_.texture.height / 2);
    const auto client_1_txt_pos_x = tex_center.X - tex_center.X / 2;
    const auto client_2_txt_pos_x = tex_center.X + tex_center.X / 2;

    const char* client_idx_txt = raylib::TextFormat("Client %i", client_idx_);
    raylib::DrawRaylibText(client_idx_txt,
        tex_center.X - raylib::MeasureText(client_idx_txt, kFontSize) / 2, 
        75.f,
        kFontSize, raylib::kWhite);

    raylib::DrawRaylibText("Client 1",
             client_1_txt_pos_x - raylib::MeasureText(client_idx_txt, kFontSize) / 2,
             175.f, kFontSize, raylib::kWhite);
    raylib::DrawRaylibText("Client 2",
             client_2_txt_pos_x - raylib::MeasureText(client_idx_txt, kFontSize) / 2,
             175.f, kFontSize, raylib::kWhite);

    for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
      int text_pos_x = i == 0 ? client_1_txt_pos_x : client_2_txt_pos_x;

      if (i == 0)
        text_pos_x = client_idx_ == 1 ? client_1_txt_pos_x : client_2_txt_pos_x;
      else
        text_pos_x = client_idx_ == 1 ? client_2_txt_pos_x : client_1_txt_pos_x;

      const inputs::PlayerInput inputs = i == 0 ? inputs_ : other_client_inputs_;


      raylib::Color key_color = inputs & static_cast<inputs::PlayerInput>(
                                  inputs::PlayerInputType::kUp)
                                  ? raylib::kWhite
                                  : raylib::kDarkGray;
      raylib::DrawRaylibText("UP", text_pos_x - raylib::MeasureText("UP", kFontSize) / 2,
               kStartTextPosY + 1 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInput>(
                                inputs::PlayerInputType::kLeft)
                      ? raylib::kWhite
                      : raylib::kDarkGray;
      raylib::DrawRaylibText("LEFT", text_pos_x - raylib::MeasureText("LEFT", kFontSize) / 2,
               kStartTextPosY + 2 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInput>(
                                inputs::PlayerInputType::kDown)
                      ? raylib::kWhite
                      : raylib::kDarkGray;
      raylib::DrawRaylibText("DOWN", text_pos_x - raylib::MeasureText("DOWN", kFontSize) / 2,
               kStartTextPosY + 3 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInput>(
                                inputs::PlayerInputType::kRight)
                      ? raylib::kWhite
                      : raylib::kDarkGray;
      raylib::DrawRaylibText("RIGHT", text_pos_x - raylib::MeasureText("RIGHT", kFontSize) / 2,
               kStartTextPosY + 4 * kTextOffsetY, kFontSize, key_color);

      key_color = inputs & static_cast<inputs::PlayerInput>(
                               inputs::PlayerInputType::kShoot)
                      ? raylib::kWhite
                      : raylib::kDarkGray;

      raylib::DrawRaylibText("SHOOT", text_pos_x - raylib::MeasureText("SHOOT", kFontSize) / 2,
               kStartTextPosY + 5 * kTextOffsetY, kFontSize, key_color);
    }
  }
  raylib::EndTextureMode();
}

void SimulationClient::Deinit() noexcept {
  raylib::UnloadRenderTexture(render_texture_);
}

void SimulationClient::RaiseEvent(bool reliable,
                                  const ExitGames::Common::Hashtable& data,
                                  EventCode event_code) noexcept {
  
}

void SimulationClient::ReceiveEvent(int player_nr, EventCode event_code,
  const ExitGames::Common::Object& event_content) noexcept {
  
}

//void SimulationClient::SendInputs(const inputs::FrameInput& inputs) noexcept {
//  const auto delay = Math::Random::Range(server_->min_packet_delay, 
//                                              server_->max_packet_delay);
//  const inputs::DebugInput debug_inputs{inputs, delay, client_idx_};
//  if (inputs.input != 0)
//  {
//    std::cout << "client " << client_idx_ << " sent input from frame nbr " << 
//   " " << inputs.frame_nbr << '\n';
//  }
//  server_->ReceiveInputs(debug_inputs);
//}
//
//void SimulationClient::ReceiveInputs(inputs::FrameInput inputs) noexcept {
//  other_client_inputs_ = inputs.input;
//
//  if (inputs.input != 0)
//  {
//    std::cout << "client " << client_idx_ << " received input from frame nbr " << 
//    " " << inputs.frame_nbr << '\n';
//  }
//}