#include "debug_inputs_app.h"

#include <imgui.h>

#include "engine.h"

void DebugInputsApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  client_1_.Init(texture_size);
  client_2_.Init(texture_size);
}

void DebugInputsApp::Update() noexcept {
  client_1_.Update();
  client_2_.Update();
}

void DebugInputsApp::Draw() noexcept {
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
  DrawTextureRec(client_1_tex,
                 Rectangle{0, 0, static_cast<float>(client_2_tex.width),
                           static_cast<float>(-client_2_tex.height)},
                 Vector2{static_cast<float>(client_2_tex.width), 0}, WHITE);

}

void DebugInputsApp::DrawImGui() noexcept {
  ImGui::ShowDemoWindow();
}

void DebugInputsApp::TearDown() noexcept {
  client_1_.Deinit();
  client_2_.Deinit();
}

void DebugInputsClient::Init(Math::Vec2I render_tex_size) noexcept {
  render_texture_ = LoadRenderTexture(render_tex_size.X, render_tex_size.Y);
}

void DebugInputsClient::Update() noexcept {
  if (IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    render_texture_.texture.width = new_tex_size.X;
    render_texture_.texture.height = new_tex_size.Y;
  }
}

void DebugInputsClient::Draw() noexcept {
  BeginTextureMode(render_texture_);
  {
    ClearBackground(BLACK);
    const auto tex_center = Math::Vec2I(render_texture_.texture.width / 2,
                                        render_texture_.texture.height / 2);
    DrawRectangle(tex_center.X, tex_center.Y, 100, 100, WHITE);
  }
  EndTextureMode();
}

void DebugInputsClient::Deinit() noexcept {
  UnloadRenderTexture(render_texture_);
}
