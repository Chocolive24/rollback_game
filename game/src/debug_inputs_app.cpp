#include "debug_inputs_app.h"

#include <imgui.h>

#include "engine.h"

void DebugInputsApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  client_1_tex_ = LoadRenderTexture(texture_size.X, texture_size.Y);
  client_2_tex_ = LoadRenderTexture(texture_size.X, texture_size.Y);
}

void DebugInputsApp::Update() noexcept {
  if (IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    client_1_tex_.texture.width = new_tex_size.X;
    client_1_tex_.texture.height = new_tex_size.Y;

    client_2_tex_.texture.width = new_tex_size.X;
    client_2_tex_.texture.height = new_tex_size.Y;
  }
}

void DebugInputsApp::Draw() noexcept {
  BeginTextureMode(client_1_tex_);
  {
    ClearBackground(RED);
    const auto tex_center = Math::Vec2I(client_1_tex_.texture.width / 2,
                                        client_1_tex_.texture.height / 2);
    DrawRectangle(tex_center.X, tex_center.Y, 100, 100, WHITE);
  }
  EndTextureMode();

  // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
  // (left-bottom)
  DrawTextureRec(client_1_tex_.texture,
                 Rectangle{0, 0, static_cast<float>(client_1_tex_.texture.width),
                             static_cast<float>(-client_1_tex_.texture.height)},
                 Vector2{0, 0}, WHITE);

  BeginTextureMode(client_2_tex_); {
    ClearBackground(BLUE);
    const auto tex_center = Math::Vec2I(client_2_tex_.texture.width / 2,
                                        client_2_tex_.texture.height / 2);
    DrawRectangle(tex_center.X, tex_center.Y, 100, 100, WHITE);
  }
  EndTextureMode();

  // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
  // (left-bottom)
  DrawTextureRec(
      client_2_tex_.texture,
      Rectangle{0, 0, static_cast<float>(client_2_tex_.texture.width),
                static_cast<float>(-client_2_tex_.texture.height)},
      Vector2{static_cast<float>(client_2_tex_.texture.width), 0}, WHITE);
}

void DebugInputsApp::DrawImGui() noexcept {
  ImGui::ShowDemoWindow();
}

void DebugInputsApp::TearDown() noexcept {
  UnloadRenderTexture(client_1_tex_);
  UnloadRenderTexture(client_2_tex_);
}
