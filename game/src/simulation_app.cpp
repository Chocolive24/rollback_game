#include "simulation_app.h"
#include "engine.h"

#include <imgui.h>

void SimulationApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    clients_[i].Init(i + 1, texture_size);
  }

  clients_[0].RegisterOtherClient(&clients_[1]);
  clients_[1].RegisterOtherClient(&clients_[0]);

  game_manager_.Init();
  game_renderer_.Init();
}

void SimulationApp::Update() noexcept {
  for (auto& client : clients_)
  {
    client.Update();
  }

  game_manager_.Update();
}

void SimulationApp::Draw() noexcept {
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    auto& client = clients_[i];
    client.Draw();

    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
    // (left-bottom)
    const auto client_tex = client.render_texture().texture;
    const raylib::Vector2 pos = i == 0
                                  ? raylib::Vector2{0, 0}
                                  : raylib::Vector2{static_cast<float>(client_tex.width), 0};
    
    DrawTextureRec(client_tex,
                   raylib::Rectangle{0, 0, static_cast<float>(client_tex.width),
                             static_cast<float>(-client_tex.height)},
                   pos, raylib::kWhite);
    //game_renderer_.Draw(client.render_texture());
  }
}

void SimulationApp::DrawImGui() noexcept {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin("Mock network values.");
  {
    ImGui::SliderFloat("Min delay", &SimulationClient::min_packet_delay, 0.f, 1.f);
    ImGui::SliderFloat("Max delay", &SimulationClient::max_packet_delay, 0.f, 1.f);
    ImGui::SliderFloat("PacketLossPercentage", &SimulationClient::packet_loss_percentage, 0.f,
                       1.f);
  }
  ImGui::End();
}

void SimulationApp::TearDown() noexcept {
  for (auto& client : clients_)
  {
    client.Deinit();
  }

  game_manager_.Deinit();
  game_renderer_.Deinit();
}
