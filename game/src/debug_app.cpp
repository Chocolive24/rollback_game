#include "debug_app.h"
#include "engine.h"

#include <imgui.h>

void DebugApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  client_1_.Init(1, texture_size);
  client_2_.Init(2, texture_size);
}

void DebugApp::Update() noexcept {
  client_1_.Update();
  client_2_.Update();
}

void DebugApp::Draw() noexcept {
  client_1_.Draw();

  // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
  // (left-bottom)
  const auto client_1_tex = client_1_.render_texture().texture;
  DrawTextureRec(client_1_tex,
                 Rectangle{0, 0, static_cast<float>(client_1_tex.width),
                           static_cast<float>(-client_1_tex.height)},
                 Vector2{0, 0}, WHITE);

  client_2_.Draw();

  // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
  // (left-bottom)
  const auto client_2_tex = client_2_.render_texture().texture;
  DrawTextureRec(client_2_tex,
                 Rectangle{0, 0, static_cast<float>(client_2_tex.width),
                           static_cast<float>(-client_2_tex.height)},
                 Vector2{static_cast<float>(client_2_tex.width), 0}, WHITE);
}

void DebugApp::DrawImGui() noexcept { ImGui::ShowDemoWindow(); }

void DebugApp::TearDown() noexcept {
  client_1_.Deinit();
  client_2_.Deinit();
}
