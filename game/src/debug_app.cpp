#include "debug_app.h"
#include "engine.h"

#include <imgui.h>

void DebugApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    clients_[i].Init(i + 1, texture_size);
    clients_[i].RegisterServer(&server_);
  }

  server_.RegisterClients(&clients_[0], &clients_[1]);
}

void DebugApp::Update() noexcept {
  for (auto& client : clients_)
  {
    client.Update();
  }

  server_.Update();
}

void DebugApp::Draw() noexcept {
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    auto& client = clients_[i];
    client.Draw();

    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
    // (left-bottom)
    const auto client_tex = client.render_texture().texture;
    const Vector2 pos = i == 0
                            ? Vector2{0, 0}
                            : Vector2{static_cast<float>(client_tex.width), 0};
    
    DrawTextureRec(client_tex,
                   Rectangle{0, 0, static_cast<float>(client_tex.width),
                             static_cast<float>(-client_tex.height)},
                   pos, WHITE);
  }
}

void DebugApp::DrawImGui() noexcept { server_.DrawImGui(); }

void DebugApp::TearDown() noexcept {
  for (auto& client : clients_)
  {
    client.Deinit();
  }

  server_.TearDown();
}
