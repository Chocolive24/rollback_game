#include "simulation_client.h"
#include "engine.h"
#include "game_constants.h"
#include "Random.h"

#include <iostream>

float SimulationClient::min_packet_delay = 1.f;
float SimulationClient::max_packet_delay = 1.f;
float SimulationClient::packet_loss_percentage = 0.0f;

void SimulationClient::Init(int input_profile_id, PlayerId player_id) noexcept {

  input_profile_id_ = input_profile_id;
  player_id_ = player_id;

  waiting_input_queue.reserve(kBaseInputSize);

  game_manager_.Init(player_id);
  game_renderer_.Init();
}

void SimulationClient::RegisterOtherClient(SimulationClient* other_client) noexcept {
  other_client_ = other_client;
}

void SimulationClient::Update() noexcept {
  static float fixed_timer = game_constants::kFixedDeltaTime;
  fixed_timer += raylib::GetFrameTime();

  while (fixed_timer >= game_constants::kFixedDeltaTime) {
    FixedUpdate();
    fixed_timer -= game_constants::kFixedDeltaTime;
  }
}

void SimulationClient::FixedUpdate() noexcept {
  current_frame_++;

  const auto input = inputs::GetPlayerInput(input_profile_id_);

 /* if (input == 0)
  {
    input = 2;
  }*/

  const inputs::FrameInput frame_input{input, current_frame_};
  const auto delay = Math::Random::Range(min_packet_delay, max_packet_delay);
  game_manager_.SetPlayerInput(frame_input, player_id_);

  ExitGames::Common::Hashtable event_data;
  event_data.put(static_cast<nByte>(EventKey::kPlayerInput), input);
  event_data.put(static_cast<nByte>(EventKey::kFrameNbr), current_frame_);
  event_data.put(static_cast<nByte>(EventKey::kDelay), delay);
  RaiseEvent(false, EventCode::kInput, event_data);

  auto it = waiting_input_queue.begin();
  while (it != waiting_input_queue.end()) {
    it->delay -= game_constants::kFixedDeltaTime;

    if (it->delay <= 0.f) {
      game_manager_.SetRemotePlayerInput(it->frame_input,
                                   other_client_->player_id());
      it = waiting_input_queue.erase(it);
     
    }
    else {
      ++it;
    }
  }

  game_manager_.FixedUpdate();
}

void SimulationClient::Draw(const raylib::RenderTexture2D& render_target) noexcept {
  game_renderer_.Draw(render_target);
}

void SimulationClient::Deinit() noexcept {
  game_manager_.Deinit();
  game_renderer_.Deinit();
}

void SimulationClient::RaiseEvent(bool reliable,
                                  EventCode event_code,
                                  const ExitGames::Common::Hashtable& event_data) noexcept {
  if (Math::Random::Range(0.f, 1.f) < packet_loss_percentage) {
    return;
  }

  if (other_client_ != nullptr){
    other_client_->ReceiveEvent(player_id_, event_code, event_data);
  }
}

void SimulationClient::ReceiveEvent(int player_nr, EventCode event_code,
                                    const ExitGames::Common::Hashtable& event_content) noexcept {
 // std::cout << "received: " << event_content.toString().UTF8Representation().cstr() << '\n';

  inputs::SimulationInput simulation_input{};

  const auto input_value =
      event_content.getValue(static_cast<nByte>(EventKey::kPlayerInput));
  simulation_input.frame_input.input =
      *ExitGames::Common::ValueObject<inputs::PlayerInput>(input_value).getDataAddress();

  //if (simulation_input.frame_input.input != 0) {
  //  std::cout << "client " << (int)player_id_ <<  "received input: "
  //  << (int)simulation_input.frame_input.input << '\n';
  //}

  const auto frame_value = event_content.getValue(
      static_cast<nByte>(EventKey::kFrameNbr));
  simulation_input.frame_input.frame_nbr =
      *ExitGames::Common::ValueObject<short>(frame_value).getDataAddress();

  const auto delay_value =
      event_content.getValue(static_cast<nByte>(EventKey::kDelay));
  simulation_input.delay =
      *ExitGames::Common::ValueObject<float>(delay_value).getDataAddress();

  waiting_input_queue.push_back(simulation_input);
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