#include "debug_server.h"

#include <imgui.h>

#include "debug_client.h"
#include "Random.h"

void DebugServer::Setup() noexcept { game_manager_.Init(); }

void DebugServer::Update() noexcept {
  game_manager_.Update();
}

void DebugServer::DrawImGui() noexcept {
  ImGui::Begin("Mock network values."); {
    ImGui::SliderFloat("Delay", &delay_, 0.f, 1.f);
    ImGui::SliderFloat("PacketLossPercentage", &packet_loss_percentage_, 0.f, 1.f);
  }
  ImGui::End();
}

void DebugServer::TearDown() noexcept { game_manager_.Deinit(); }

void DebugServer::SendInputs(inputs::FrameInputs,
                             DebugClient* client) noexcept {}

void DebugServer::ReceiveInputs(inputs::FrameInputs inputs, int client_idx) noexcept {
  if (Math::Random::Range(0.f, 1.f) <= packet_loss_percentage_) {
    return;
  }

  //while (delay_time_ < delay_) {
  //  delay_time_ += GetFrameTime();
  //}

  delay_time_ = 0;
  const auto other_client_idx = client_idx == 1 ? 1 : 0;
  clients_[other_client_idx]->ReceiveInputs(inputs);
}
