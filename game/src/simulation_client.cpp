#include "simulation_client.h"
#include "game_constants.h"
#include "Random.h"

#include <iostream>

float SimulationClient::min_packet_delay = 0.01f;
float SimulationClient::max_packet_delay = 0.3f;
float SimulationClient::packet_loss_percentage = 0.1f;

void SimulationClient::Init(int input_profile_id, PlayerId player_id) noexcept {

  input_profile_id_ = input_profile_id;
  player_id_ = player_id;

  waiting_input_queue.reserve(kBaseInputSize);

  game_manager_.Init(player_id);
  game_renderer_.Init();

  rollback_manager_.RegisterGameManager(&game_manager_);

  timer_.Init();
}

void SimulationClient::RegisterOtherClient(SimulationClient* other_client) noexcept {
  other_client_ = other_client;
}

void SimulationClient::Update() noexcept {
  timer_.Tick();
  fixed_timer_ += timer_.DeltaTime();
  while (fixed_timer_ >= game_constants::kFixedDeltaTime) {
    FixedUpdate();
    fixed_timer_ -= game_constants::kFixedDeltaTime;
  }
}

void SimulationClient::SendInputEvent() {
  const auto input = inputs::GetPlayerInput(input_profile_id_);

  const inputs::FrameInput frame_input{input, current_frame_};
  rollback_manager_.SetLocalPlayerInput(frame_input, player_id_);

  inputs_.push_back(input);
  frames_.push_back(current_frame_);

  ExitGames::Common::Hashtable event_data;
  const auto delay = Math::Random::Range(min_packet_delay, max_packet_delay);
  event_data.put<nByte, inputs::PlayerInput*>(static_cast<nByte>(EventKey::kPlayerInput), 
      inputs_.data(), inputs_.size());
  event_data.put<nByte, FrameNbr*>(static_cast<nByte>(EventKey::kFrameNbr), 
      frames_.data(), frames_.size());
  event_data.put(static_cast<nByte>(EventKey::kDelay), delay);

  RaiseEvent(false, EventCode::kInput, event_data);
}

void SimulationClient::PollInputPackets() {
  auto it = waiting_input_queue.begin();
  while (it != waiting_input_queue.end()) {
    it->delay -= game_constants::kFixedDeltaTime;

    if (it->delay <= 0.f) {
      rollback_manager_.SetRemotePlayerInput(it->frame_inputs,
                                         other_client_->player_id());

      if (player_id_ == kMasterClientId)
      {
        auto frame_to_confirm_it = std::find_if(
            it->frame_inputs.begin(), it->frame_inputs.end(),
            [this](inputs::FrameInput frame_input) {
              return frame_input.frame_nbr ==
                     rollback_manager_.frame_to_confirm();
        });

        while (frame_to_confirm_it != it->frame_inputs.end())
        {
          const auto frame_to_confirm = *frame_to_confirm_it;

            const int check_sum =
              rollback_manager_.ComputeFrameToConfirmChecksum();

          ExitGames::Common::Hashtable event_check_sum;

          event_check_sum.put(static_cast<nByte>(EventKey::kCheckSum),
                              check_sum);
          const auto& frame_nbr_it = std::find(frames_.begin(), frames_.end(),
                                              frame_to_confirm.frame_nbr);
          const auto& dist = std::distance(frames_.begin(), frame_nbr_it);

          event_check_sum.put<nByte, inputs::PlayerInput*>(
              static_cast<nByte>(EventKey::kPlayerInput), inputs_.data(), dist + 1);
          event_check_sum.put<nByte, FrameNbr*>(
              static_cast<nByte>(EventKey::kFrameNbr), frames_.data(), dist + 1);
          event_check_sum.put(static_cast<nByte>(EventKey::kDelay),
                              max_packet_delay);


          RaiseEvent(true, EventCode::kFrameConfirmation, event_check_sum);

          rollback_manager_.ConfirmFrame(frame_to_confirm.frame_nbr);

 /*         inputs_.erase(inputs_.begin());
          frames_.erase(frames_.begin());*/

          ++frame_to_confirm_it;
        }
      }

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
    frame_it->delay -= game_constants::kFixedDeltaTime;

    if (frame_it->delay <= 0.f) {

      if (player_id_ != kMasterClientId) {

          // If we did not receive the inputs before the frame to confirm, add them.
          if (rollback_manager_.last_remote_input_frame() < frame_it->frame_inputs.back().frame_nbr)
          {
            auto it = std::find_if(
              frame_it->frame_inputs.begin(), frame_it->frame_inputs.end(),
              [this](inputs::FrameInput frame_input) {
                return frame_input.frame_nbr ==
                       rollback_manager_.last_remote_input_frame();
              });

            std::vector<inputs::FrameInput> inputs(it, frame_it->frame_inputs.end());

            rollback_manager_.SetRemotePlayerInput(inputs, other_client_->player_id());
          }

          const int check_sum =
              rollback_manager_.ComputeFrameToConfirmChecksum();

          if (check_sum != frame_it->check_sum) {
            std::cerr << "Not same checksum for frame: " << rollback_manager_.frame_to_confirm()
                      << '\n';
            return;
          }

          std::cout << "Checksum equal for frame: "
                    << rollback_manager_.frame_to_confirm()
                    << '\n';
          rollback_manager_.ConfirmFrame(rollback_manager_.frame_to_confirm());
          inputs_.erase(inputs_.begin());
          frames_.erase(frames_.begin());
      }

      
      frame_it = waiting_frame_queue_.erase(frame_it);
    }

    else {
      ++frame_it;
    }
  }
}

void SimulationClient::FixedUpdate() noexcept {
  current_frame_++;

  SendInputEvent();
  PollInputPackets();
  PollConfirmFramePackets();

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
  if (!reliable && Math::Random::Range(0.f, 1.f) < packet_loss_percentage) {
    return;
  }

  if (other_client_ != nullptr){
    other_client_->ReceiveEvent(player_id_, event_code, event_data);
  }
}

void SimulationClient::ReceiveEvent(int player_nr, EventCode event_code,
                                    const ExitGames::Common::Hashtable& event_content) noexcept {
  switch (event_code)
  {
    case EventCode::kInput: {
      inputs::SimulationInput simulation_input{};

      const auto input_value =
          event_content.getValue(static_cast<nByte>(EventKey::kPlayerInput));

      const inputs::PlayerInput* inputs =
          ExitGames::Common::ValueObject<inputs::PlayerInput*>(input_value).getDataCopy();

      const int inputs_count =
          *ExitGames::Common::ValueObject<inputs::PlayerInput*>(input_value).getSizes();

      const auto frame_value =
          event_content.getValue(static_cast<nByte>(EventKey::kFrameNbr));

       const FrameNbr* frames =
          ExitGames::Common::ValueObject<FrameNbr*>(frame_value).getDataCopy();

      for (int i = 0; i < inputs_count; i++) {
        inputs::FrameInput frame_input{inputs[i], frames[i]};
        simulation_input.frame_inputs.push_back(frame_input);
      }

      const auto delay_value =
          event_content.getValue(static_cast<nByte>(EventKey::kDelay));
      simulation_input.delay =
          ExitGames::Common::ValueObject<float>(delay_value).getDataCopy();

      waiting_input_queue.push_back(simulation_input);

      ExitGames::Common::MemoryManagement::deallocateArray(inputs);
      ExitGames::Common::MemoryManagement::deallocateArray(frames);

      break;
    }
    case EventCode::kFrameConfirmation: {
      FrameToConfirm frame_to_confirm{};

      const auto check_sum_value =
          event_content.getValue(static_cast<nByte>(EventKey::kCheckSum));
      frame_to_confirm.check_sum =
          ExitGames::Common::ValueObject<int>(check_sum_value).getDataCopy();

      //const auto frame_value =
      //    event_content.getValue(static_cast<nByte>(EventKey::kFrameNbr));
      //frame_to_confirm.frame_nbr =
      //    ExitGames::Common::ValueObject<short>(frame_value).getDataCopy();

      const auto input_value =
          event_content.getValue(static_cast<nByte>(EventKey::kPlayerInput));

      const inputs::PlayerInput* inputs =
          ExitGames::Common::ValueObject<inputs::PlayerInput*>(input_value)
              .getDataCopy();

      const int inputs_count =
          *ExitGames::Common::ValueObject<inputs::PlayerInput*>(input_value)
               .getSizes();

      const auto frame_value =
          event_content.getValue(static_cast<nByte>(EventKey::kFrameNbr));

      const FrameNbr* frames =
          ExitGames::Common::ValueObject<FrameNbr*>(frame_value).getDataCopy();

      for (int i = 0; i < inputs_count; i++) {
        inputs::FrameInput frame_input{inputs[i], frames[i]};
        frame_to_confirm.frame_inputs.push_back(frame_input);
      }


      const auto delay_value =
          event_content.getValue(static_cast<nByte>(EventKey::kDelay));
      frame_to_confirm.delay =
          ExitGames::Common::ValueObject<float>(delay_value).getDataCopy();

      waiting_frame_queue_.push_back(frame_to_confirm);

      break;
    }
  }

 
}