#include "simulation_client.h"
#include "engine.h"
#include "game_constants.h"
#include "Random.h"

#include <iostream>
#include <Common-cpp/inc/UTF8String.h>

float SimulationClient::min_packet_delay = 0.1f;
float SimulationClient::max_packet_delay = 0.3f;
float SimulationClient::packet_loss_percentage = 0.1f;

void SimulationClient::Init(int local_player_id,
                            Math::Vec2I render_tex_size) noexcept {

  local_player_id_ = local_player_id;
  render_texture_ = raylib::LoadRenderTexture(render_tex_size.X, render_tex_size.Y);

  local_inputs_.reserve(kBaseInputSize);
  other_client_inputs_.reserve(kBaseInputSize);
}

void SimulationClient::RegisterOtherClient(SimulationClient* other_client) noexcept {
  other_client_ = other_client;
}

void SimulationClient::Update() noexcept {
  current_frame_++;

  if (raylib::IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    UnloadRenderTexture(render_texture_);
    render_texture_ = raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
  }

  const auto input = inputs::GetPlayerInput(local_player_id_);
  const inputs::FrameInput frame_input{input, current_frame_};
  const auto delay = Math::Random::Range(min_packet_delay, max_packet_delay);
  const inputs::SimulationInput simulation_input{frame_input, delay};
  local_inputs_.push_back(simulation_input);

  ExitGames::Common::Hashtable event_data;
  event_data.put(static_cast<nByte>(EventKey::kInput), input);
  RaiseEvent(false, EventCode::kInput, event_data);
  //SendInputs(inputs::FrameInput{inputs_, current_frame_});
}

void SimulationClient::Draw() noexcept {
  raylib::BeginTextureMode(render_texture_);
  {
    raylib::ClearBackground(raylib::kBlack);

    const auto tex_center = Math::Vec2I(render_texture_.texture.width / 2,
                                        render_texture_.texture.height / 2);
    const auto client_1_txt_pos_x = tex_center.X - tex_center.X / 2;
    const auto client_2_txt_pos_x = tex_center.X + tex_center.X / 2;

    const char* client_idx_txt = raylib::TextFormat("Client %i", local_player_id_);
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
        text_pos_x =
            local_player_id_ == 1 ? client_1_txt_pos_x : client_2_txt_pos_x;
      else
        text_pos_x =
            local_player_id_ == 1 ? client_2_txt_pos_x : client_1_txt_pos_x;

      inputs::PlayerInput inputs = 0;
          //i == 0 ? local_inputs_.back().frame_input.input
          //       : other_client_inputs_.back().frame_input.input;

      if (i == 0 && !local_inputs_.empty()) {
        inputs = local_inputs_.back().frame_input.input;
      }

      if (i == 1 && !other_client_inputs_.empty())
      {
        inputs = other_client_inputs_.back().frame_input.input;
      }

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
                                  EventCode event_code,
                                  const ExitGames::Common::Hashtable& event_data) noexcept {
  if (other_client_ != nullptr){
    other_client_->ReceiveEvent(0, event_code, event_data);
  }
}

void SimulationClient::ReceiveEvent(int player_nr, EventCode event_code,
                                    const ExitGames::Common::Hashtable& event_content) noexcept {

  std::cout << "received: " << event_content.toString().UTF8Representation().cstr() << '\n';
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