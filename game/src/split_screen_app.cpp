#include "split_screen_app.h"
#include "engine.h"

void SplitScreenApp::Setup() noexcept {
  auto texture_size = Engine::window_size();
  texture_size.X /= 2;
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    clients_[i].Init(i);
    render_targets_[i] =
        raylib::LoadRenderTexture(texture_size.X, texture_size.Y);
  }

  input::FrameInput::registerType();
}

void SplitScreenApp::Update() noexcept {
  for (auto& client : clients_) {
    client.Update();
  }
}

void SplitScreenApp::Draw() noexcept {
  for (int i = 0; i < game_constants::kMaxPlayerCount; i++) {
    auto& client = clients_[i];

    if (raylib::IsWindowResized()) {
      auto new_tex_size = Engine::window_size();
      new_tex_size.X /= 2;
      raylib::UnloadRenderTexture(render_targets_[i]);
      render_targets_[i] =
          raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
    }

    client.Draw(render_targets_[i]);

    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates
    // (left-bottom)
    const auto& render_tex = render_targets_[i].texture;
    const raylib::Vector2 pos =
        i == 0 ? raylib::Vector2{0, 0}
               : raylib::Vector2{static_cast<float>(render_tex.width), 0};

    DrawTextureRec(render_tex,
                   raylib::Rectangle{0, 0, static_cast<float>(render_tex.width),
                                     static_cast<float>(-render_tex.height)},
                   pos, raylib::kWhite);
  }
}

void SplitScreenApp::DrawImGui() noexcept {
  for (auto& client : clients_) {
    client.DrawImGui();
  }
}

void SplitScreenApp::TearDown() noexcept {
  for (auto& client : clients_) {
    client.Deinit();
  }

  for (const auto& render_target : render_targets_) {
    raylib::UnloadRenderTexture(render_target);
  }

  input::FrameInput::unregisterType();
}
