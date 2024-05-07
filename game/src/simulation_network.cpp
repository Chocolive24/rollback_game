#include "simulation_network.h"

#include "Random.h"

float SimulationNetwork::min_packet_delay = 0.01f;
float SimulationNetwork::max_packet_delay = 0.03f;
float SimulationNetwork::packet_loss_percentage = 0.1f;

void SimulationNetwork::Service() noexcept {
  PollInputPackets();
  PollConfirmFramePackets();
}

void SimulationNetwork::RaiseEvent(
    bool reliable, NetworkEventCode event_code,
    const ExitGames::Common::Hashtable& event_data) noexcept {
  if (!reliable && Math::Random::Range(0.f, 1.f) < packet_loss_percentage) {
    return;
  }

  const auto input_value =
      event_data.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput));

  const input::FrameInput* inputs =
      ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
          .getDataCopy();

  const int inputs_count =
      *ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
           .getSizes();

  ExitGames::Common::Hashtable simulated_event = event_data;
  const auto delay =
      reliable ? 0.08f
               : Math::Random::Range(min_packet_delay, max_packet_delay);
  simulated_event.put(static_cast<nByte>(NetworkEventKey::kDelay), delay);

  if (other_client_network_ != nullptr) {
    other_client_network_->ReceiveEvent(0, event_code,
                                simulated_event);
  }
}

void SimulationNetwork::ReceiveEvent(
    int player_nr, NetworkEventCode event_code,
    const ExitGames::Common::Hashtable& event_content) noexcept {
  switch (event_code) {
    case NetworkEventCode::kInput: {
      SimulationInput simulation_input{};

      const auto input_value = event_content.getValue(
          static_cast<nByte>(NetworkEventKey::kPlayerInput));

      const input::FrameInput* inputs =
          ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
              .getDataCopy();

      const int inputs_count =
          *ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
               .getSizes();

      for (int i = 0; i < inputs_count; i++) {
        input::FrameInput frame_input{inputs[i]};
        simulation_input.frame_inputs.push_back(frame_input);
      }

      const auto delay_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kDelay));
      simulation_input.delay =
          ExitGames::Common::ValueObject<float>(delay_value).getDataCopy();

      waiting_input_queue_.push_back(simulation_input);

      ExitGames::Common::MemoryManagement::deallocateArray(inputs);

      break;
    }
    case NetworkEventCode::kFrameConfirmation: {
     /* if (online_game_manager_.player_id() == kMasterClientId) {
        const SimulationFrameToConfirm f{0, {}, 0.08f};
        waiting_frame_queue_.push_back(f);
        break;
      }*/

      SimulationFrameToConfirm frame_to_confirm{};

      const auto check_sum_value = event_content.getValue(
          static_cast<nByte>(NetworkEventKey::kCheckSum));
      frame_to_confirm.check_sum =
          ExitGames::Common::ValueObject<int>(check_sum_value).getDataCopy();

      const auto input_value = event_content.getValue(
          static_cast<nByte>(NetworkEventKey::kPlayerInput));

      const input::FrameInput* inputs =
          ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
              .getDataCopy();

      const int inputs_count =
          *ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
               .getSizes();

      for (int i = 0; i < inputs_count; i++) {
        frame_to_confirm.frame_inputs.push_back(inputs[i]);
      }

      const auto delay_value =
          event_content.getValue(static_cast<nByte>(NetworkEventKey::kDelay));
      frame_to_confirm.delay =
          ExitGames::Common::ValueObject<float>(delay_value).getDataCopy();

      waiting_frame_queue_.push_back(frame_to_confirm);

      ExitGames::Common::MemoryManagement::deallocateArray(inputs);
      break;
    }
  }
}



void SimulationNetwork::PollInputPackets() {
  auto it = waiting_input_queue_.begin();
  while (it != waiting_input_queue_.end()) {
    it->delay -= game_constants::kFixedDeltaTime;

    if (it->delay <= 0.f) {
      ExitGames::Common::Hashtable event_data;
      event_data.put(static_cast<nByte>(NetworkEventKey::kPlayerInput),
                     it->frame_inputs.data(),
                     static_cast<int>(it->frame_inputs.size()));

      NetworkEvent network_event{NetworkEventCode::kInput, event_data};
      client_->OnNetworkEventReceived(network_event);

      it = waiting_input_queue_.erase(it);
    } else {
      ++it;
    }
  }
}

void SimulationNetwork::PollConfirmFramePackets() {
  auto frame_it = waiting_frame_queue_.begin();
  while (frame_it != waiting_frame_queue_.end()) {
    frame_it->delay -= raylib::GetFrameTime();

    if (frame_it->delay <= 0.f) {
      ExitGames::Common::Hashtable event_data;
      event_data.put(static_cast<nByte>(NetworkEventKey::kCheckSum),
                     frame_it->check_sum);
      event_data.put(static_cast<nByte>(NetworkEventKey::kPlayerInput),
                     frame_it->frame_inputs.data(),
                     static_cast<int>(frame_it->frame_inputs.size()));

      NetworkEvent network_event{NetworkEventCode::kFrameConfirmation,
                                 event_data};
      client_->OnNetworkEventReceived(network_event);

      frame_it = waiting_frame_queue_.erase(frame_it);
    }

    else {
      ++frame_it;
    }
  }
}