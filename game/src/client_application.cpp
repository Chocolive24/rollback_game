#include "client_application.h"
#include "engine.h"

void ClientApplication::Setup() noexcept {
  client_.Init(game_constants::kLocalPlayer1InputId);

  render_texture_ = raylib::LoadRenderTexture(raylib::GetScreenWidth(),
                                              raylib::GetScreenHeight());

  input::FrameInput::registerType();
}

void ClientApplication::Update() noexcept {
  client_.Update();
}

void ClientApplication::Draw() noexcept {
  if (raylib::IsWindowResized()) {
    const auto new_tex_size = Engine::window_size();
    UnloadRenderTexture(render_texture_);
    render_texture_ = raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
  }

  client_.Draw(render_texture_, raylib::Vector2{0.f, 0.f});

  raylib::DrawTextureRec(render_texture_.texture,
      raylib::Rectangle{0, 0, static_cast<float>(render_texture_.texture.width),
                        static_cast<float>(-render_texture_.texture.height)},
      raylib::Vector2{0.f, 0.f}, raylib::kWhite);
}

void ClientApplication::DrawImGui() noexcept {
  client_.DrawImGui();
}

void ClientApplication::TearDown() noexcept {
  client_.Deinit();

  input::FrameInput::registerType();
}