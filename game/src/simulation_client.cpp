#include "simulation_client.h"
#include "game_constants.h"
#include "Random.h"

float SimulationClient::min_packet_delay = 0.01f;
float SimulationClient::max_packet_delay = 0.03f;
float SimulationClient::packet_loss_percentage = 0.1f;

void SimulationClient::Init(int input_profile_id, PlayerId player_id) noexcept {
  waiting_input_queue.reserve(kBaseInputSize);

  online_game_manager_.Init(input_profile_id);
  online_game_manager_.SetPlayerId(player_id);
  online_game_manager_.RegisterNetworkInterface(this);

  game_renderer_.Init();
}

void SimulationClient::RegisterOtherClient(SimulationClient* other_client) noexcept {
  other_client_ = other_client;
}

void SimulationClient::Update() noexcept {
  PollInputPackets();
  PollConfirmFramePackets();

  fixed_timer_ += raylib::GetFrameTime();
  while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
    online_game_manager_.FixedUpdateCurrentFrame();
    fixed_timer_ -= game_constants::kFixedDeltaTime;
  }
}

void SimulationClient::Draw(
    const raylib::RenderTexture2D& render_target) noexcept {
  game_renderer_.Draw(render_target);
}

void SimulationClient::Deinit() noexcept {
  online_game_manager_.Deinit();
  game_renderer_.Deinit();
}

void SimulationClient::PollInputPackets() {
  auto it = waiting_input_queue.begin();
  while (it != waiting_input_queue.end()) {
    it->delay -= game_constants::kFixedDeltaTime;

    if (it->delay <= 0.f) {
      std::vector<input::PlayerInput> remote_inputs{};
      remote_inputs.reserve(it->frame_inputs.size());

      std::vector<FrameNbr> remote_frames{};
      remote_frames.reserve(it->frame_inputs.size());

      for (const auto& frame_input : it->frame_inputs)
      {
        remote_inputs.push_back(frame_input.input);
        remote_frames.push_back(frame_input.frame_nbr);
      }

       ExitGames::Common::Hashtable event_data;
       event_data.put(static_cast<nByte>(NetworkEventKey::kPlayerInput),
       remote_inputs.data(), static_cast<int>(remote_inputs.size()));
       event_data.put(static_cast<nByte>(NetworkEventKey::kFrameNbr),
           remote_frames.data(), static_cast<int>(remote_frames.size()));

      NetworkEvent network_event{NetworkEventCode::kInput, event_data};
      online_game_manager_.PushNetworkEvent(network_event);

      it = waiting_input_queue.erase(it);
    }
    else {
      ++it;
    }
  }
}

void SimulationClient::PollConfirmFramePackets() {
  auto frame_it = waiting_frame_queue_.begin();
  while (frame_it != waiting_frame_queue_.end()) {
    frame_it->delay -= raylib::GetFrameTime();

    if (frame_it->delay <= 0.f) {
      std::vector<input::PlayerInput> remote_inputs{};
      remote_inputs.reserve(frame_it->frame_inputs.size());

      std::vector<FrameNbr> remote_frames{};
      remote_frames.reserve(frame_it->frame_inputs.size());

      for (const auto& frame_input : frame_it->frame_inputs) {
        remote_inputs.push_back(frame_input.input);
        remote_frames.push_back(frame_input.frame_nbr);
      }

      ExitGames::Common::Hashtable event_data;
      event_data.put(static_cast<nByte>(NetworkEventKey::kCheckSum),
                     frame_it->check_sum);
      event_data.put(static_cast<nByte>(NetworkEventKey::kPlayerInput),
                     remote_inputs.data(),
                     static_cast<int>(remote_inputs.size()));
      event_data.put(static_cast<nByte>(NetworkEventKey::kFrameNbr),
                     remote_frames.data(),
                     static_cast<int>(remote_frames.size()));

      NetworkEvent network_event{NetworkEventCode::kFrameConfirmation, event_data};
      online_game_manager_.PushNetworkEvent(network_event);

      frame_it = waiting_frame_queue_.erase(frame_it);
    }
   
    else {
      ++frame_it;
    }
  }
}



void SimulationClient::RaiseEvent(bool reliable,
                                  NetworkEventCode event_code,
                                  const ExitGames::Common::Hashtable& event_data) noexcept {
  if (!reliable && Math::Random::Range(0.f, 1.f) < packet_loss_percentage) {
    return;
  }

  ExitGames::Common::Hashtable simulated_event = event_data;
  const auto delay =
      reliable ? 0.08f
               : Math::Random::Range(min_packet_delay, max_packet_delay);
  simulated_event.put(static_cast<nByte>(NetworkEventKey::kDelay), delay);

  if (other_client_ != nullptr){
    other_client_->ReceiveEvent(online_game_manager_.player_id(), event_code, simulated_event);
  }
}

void SimulationClient::ReceiveEvent(int player_nr, NetworkEventCode event_code,
                                    const ExitGames::Common::Hashtable& event_content) noexcept {
  switch (event_code)
  {
    case NetworkEventCode::kInput: {
      input::SimulationInput simulation_input{};

      const auto input_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput));

      const input::PlayerInput* inputs =
          ExitGames::Common::ValueObject<input::PlayerInput*>(input_value).getDataCopy();

      const int inputs_count =
          *ExitGames::Common::ValueObject<input::PlayerInput*>(input_value).getSizes();

      const auto frame_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kFrameNbr));

       const FrameNbr* frames =
          ExitGames::Common::ValueObject<FrameNbr*>(frame_value).getDataCopy();

      for (int i = 0; i < inputs_count; i++) {
        input::FrameInput frame_input{inputs[i], frames[i]};
        simulation_input.frame_inputs.push_back(frame_input);
      }

      const auto delay_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kDelay));
      simulation_input.delay =
          ExitGames::Common::ValueObject<float>(delay_value).getDataCopy();

      waiting_input_queue.push_back(simulation_input);

      ExitGames::Common::MemoryManagement::deallocateArray(inputs);
      ExitGames::Common::MemoryManagement::deallocateArray(frames);

      break;
    }
    case NetworkEventCode::kFrameConfirmation: {

      if (online_game_manager_.player_id() == kMasterClientId)
      {
        const SimulationFrameToConfirm f{0, {}, 0.08f};
        waiting_frame_queue_.push_back(f);
        break;
      }

      SimulationFrameToConfirm frame_to_confirm{};

      const auto check_sum_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kCheckSum));
      frame_to_confirm.check_sum =
          ExitGames::Common::ValueObject<int>(check_sum_value).getDataCopy();

      const auto input_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput));

      const input::PlayerInput* inputs =
          ExitGames::Common::ValueObject<input::PlayerInput*>(input_value)
              .getDataCopy();

      const int inputs_count =
          *ExitGames::Common::ValueObject<input::PlayerInput*>(input_value)
               .getSizes();

      const auto frame_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kFrameNbr));

      const FrameNbr* frames =
          ExitGames::Common::ValueObject<FrameNbr*>(frame_value).getDataCopy();

      for (int i = 0; i < inputs_count; i++) {
        input::FrameInput frame_input{inputs[i], frames[i]};
        frame_to_confirm.frame_inputs.push_back(frame_input);
      }

      const auto delay_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kDelay));
      frame_to_confirm.delay =
          ExitGames::Common::ValueObject<float>(delay_value).getDataCopy();

      waiting_frame_queue_.push_back(frame_to_confirm);

      ExitGames::Common::MemoryManagement::deallocateArray(inputs);
      ExitGames::Common::MemoryManagement::deallocateArray(frames);

      break;
    }
  }
}