#include "simulation_app.h"
#include "engine.h"

#include <imgui.h>

void SimulationApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    mock_networks_[i].RegisterClient(&clients_[i]);
    
    clients_[i].Init(i);
    clients_[i].SetClientId(i + 1); // simulate photon id which is in range [1 ; 2]
    clients_[i].RegisterNetworkInterface(&mock_networks_[i]);

    clients_[i].StartGame();

    render_targets_[i] =
        raylib::LoadRenderTexture(texture_size.X, texture_size.Y);
  }

  mock_networks_[0].RegisterOtherClientNetwork(&mock_networks_[1]);
  mock_networks_[1].RegisterOtherClientNetwork(&mock_networks_[0]);

 input::FrameInput::registerType();
}

void SimulationApp::Update() noexcept {
  for (std::size_t i = 0; i < game_constants::kMaxPlayerCount; i++) {
    mock_networks_[i].Service();
    clients_[i].Update();
  }
}

void SimulationApp::Draw() noexcept {
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    auto& client = clients_[i];

    if (raylib::IsWindowResized()) {
      auto new_tex_size = Engine::window_size();
      new_tex_size.X /= 2;
      raylib::UnloadRenderTexture(render_targets_[i]);
      render_targets_[i] =
          raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
    }

    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
    // (left-bottom)
    const auto& render_tex = render_targets_[i].texture;
    const raylib::Vector2 pos = i == 0
                                  ? raylib::Vector2{0, 0}
               : raylib::Vector2{static_cast<float>(render_tex.width), 0};

               
    client.Draw(render_targets_[i], pos);

    DrawTextureRec(render_tex,
                   raylib::Rectangle{0, 0, static_cast<float>(render_tex.width),
                                     static_cast<float>(-render_tex.height)},
                   pos, raylib::kWhite);
  }
}

void SimulationApp::DrawImGui() noexcept {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin("Mock network values.");
  {
    ImGui::SliderFloat("Min delay", &SimulationNetwork::min_packet_delay, 0.f, 1.f);
    ImGui::SliderFloat("Max delay", &SimulationNetwork::max_packet_delay, 0.f,
                       1.f);
    ImGui::SliderFloat("PacketLossPercentage",
                       &SimulationNetwork::packet_loss_percentage, 0.f,
                       1.f);
  }
  ImGui::End();
}

void SimulationApp::TearDown() noexcept {
  for (auto& client : clients_)
  {
    client.Deinit();
  }

  for (const auto& render_target : render_targets_) {
    raylib::UnloadRenderTexture(render_target);
  }

  input::FrameInput::unregisterType();
}
