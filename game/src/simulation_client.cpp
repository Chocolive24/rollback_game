#include "simulation_client.h"
#include "game_constants.h"
#include "Random.h"

#include <iostream>

float SimulationClient::min_packet_delay = 0.0f;
float SimulationClient::max_packet_delay = 0.3f;
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

void SimulationClient::SendInputEvent() {
  const auto input = inputs::GetPlayerInput(input_profile_id_);

  const inputs::FrameInput frame_input{input, current_frame_};
  game_manager_.SetPlayerInput(frame_input, player_id_);

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
      game_manager_.rollback_manager_.SetRemotePlayerInput(it->frame_inputs,
                                         other_client_->player_id());

      if (player_id_ == master_client_id)
      {
        const auto frame_to_confirm_it = std::find_if(
            it->frame_inputs.begin(), it->frame_inputs.end(),
            [this](inputs::FrameInput frame_input) {
              return frame_input.frame_nbr ==
                     game_manager_.rollback_manager_.frame_to_confirm();
        });

        // "Old packet.
        if (frame_to_confirm_it == it->frame_inputs.end())
        {
          it = waiting_input_queue.erase(it);
          return;
        }

        const int check_sum = game_manager_.rollback_manager_.SimulateUntilFrameToConfirm(
                frame_to_confirm_it->frame_nbr);

        ExitGames::Common::Hashtable event_check_sum;
        const auto check_delay = max_packet_delay;

        event_check_sum.put(static_cast<nByte>(EventKey::kCheckSum), check_sum);
        event_check_sum.put(static_cast<nByte>(EventKey::kFrameNbr),
                            frame_to_confirm_it->frame_nbr);
        event_check_sum.put(static_cast<nByte>(EventKey::kDelay), check_delay);
        RaiseEvent(true, EventCode::kFrameConfirmation, event_check_sum);

        game_manager_.rollback_manager_.ConfirmFrame(frame_to_confirm_it->frame_nbr);


      /*  if (it->frame_inputs.back().frame_nbr == 
            game_manager_.rollback_manager_.last_remote_input_frame())
        {
          const int check_sum =
              game_manager_.rollback_manager_.SimulateUntilFrameToConfirm(
                  it->frame_inputs.back().frame_nbr);

          ExitGames::Common::Hashtable event_check_sum;
          const auto check_delay =
              Math::Random::Range(min_packet_delay, max_packet_delay);
          event_check_sum.put(static_cast<nByte>(EventKey::kCheckSum),
                              check_sum);
          event_check_sum.put(static_cast<nByte>(EventKey::kFrameNbr),
                              it->frame_inputs.back().frame_nbr);
          event_check_sum.put(static_cast<nByte>(EventKey::kDelay),
                              check_delay);
          RaiseEvent(true, EventCode::kFrameConfirmation, event_check_sum);
            game_manager_.rollback_manager_.ConfirmFrame(it->frame_inputs.back().frame_nbr);

          const auto confirm_frame_it = std::find_if(
                frames_.begin(), frames_.end(), [it](FrameNbr frame_nbr) {
                  return frame_nbr == it->frame_inputs.back().frame_nbr;
          });*/
        //}
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

      if (player_id_ != master_client_id) {
        if (frame_it->frame_nbr ==
            game_manager_.rollback_manager_.frame_to_confirm())
        {
          const int check_sum =
              game_manager_.rollback_manager_.SimulateUntilFrameToConfirm(
                  frame_it->frame_nbr);

          if (check_sum != frame_it->check_sum) {
            std::cerr << "Not same checksum\n";
            std::exit(EXIT_FAILURE);
            return;
          }

          std::cout << "Checksum equal for frame: " << frame_it->frame_nbr << '\n';
          game_manager_.rollback_manager_.ConfirmFrame(frame_it->frame_nbr);
          // ExitGames::Common::Hashtable event_check_sum;
          // const auto check_delay =
          //   Math::Random::Range(min_packet_delay, max_packet_delay);
          // event_check_sum.put(static_cast<nByte>(EventKey::kCheckSum),
          //                     check_sum);
          // event_check_sum.put(static_cast<nByte>(EventKey::kFrameNbr),
          //                     frame_it->frame_nbr);
          // event_check_sum.put(static_cast<nByte>(EventKey::kDelay),
          //                     check_delay);
          // RaiseEvent(true, EventCode::kFrameConfirmation, event_check_sum);
          // game_manager_.rollback_manager_.ConfirmFrame(frame_it->frame_nbr);
        }
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

  //if (player_id_ != master_client_id)
  //{
  //  SendInputEvent();
  //}

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
 // std::cout << "received: " << event_content.toString().UTF8Representation().cstr() << '\n';

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

      
      

      //std::vector<inputs::PlayerInput> inputs_to_send{};
      //inputs_to_send.reserve(inputs_count);
      //for (int i = 0; i < inputs_count; i++)
      //{
      //  inputs_to_send.push_back(inputs[i]);
      //}

     /* simulation_input.frame_input.input =
          *ExitGames::Common::ValueObject<inputs::PlayerInput>(input_value)
               .getDataAddress();*/

       //if (simulation_input.frame_input.input != 0) {
       //  std::cout << "client " << (int)player_id_ <<  "received input: "
       //  << (int)simulation_input.frame_input.input << '\n';
       //}

      const auto frame_value =
          event_content.getValue(static_cast<nByte>(EventKey::kFrameNbr));

       const FrameNbr* frames =
          ExitGames::Common::ValueObject<FrameNbr*>(frame_value).getDataCopy();
     

      //const int frame_count =
      //     *ExitGames::Common::ValueObject<FrameNbr*>(frame_value).getSizes();

      //std::vector<FrameNbr> frames_to_send{};
      //frames_to_send.reserve(frame_count);
      //for (int i = 0; i < frame_count; i++) {
      //  frames_to_send.push_back(frames[i]);
      //}

      //simulation_input.frame_input.frame_nbr =
      //    *ExitGames::Common::ValueObject<short>(frame_value).getDataAddress();

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
          *ExitGames::Common::ValueObject<int>(check_sum_value).getDataAddress();

      const auto frame_value =
          event_content.getValue(static_cast<nByte>(EventKey::kFrameNbr));
      frame_to_confirm.frame_nbr =
          *ExitGames::Common::ValueObject<short>(frame_value).getDataAddress();

      const auto delay_value =
          event_content.getValue(static_cast<nByte>(EventKey::kDelay));
      frame_to_confirm.delay =
          *ExitGames::Common::ValueObject<float>(delay_value).getDataAddress();

      waiting_frame_queue_.push_back(frame_to_confirm);

      break;
    }
  }

 
}