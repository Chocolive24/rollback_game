#include "client_application.h"

#include "engine.h"

void ClientApplication::Setup() noexcept {
  game_manager_.Init();
  game_renderer_.Init();

  render_texture_ = raylib::LoadRenderTexture(raylib::GetScreenWidth() / 2,
                                              raylib::GetScreenHeight());
}

void ClientApplication::Update() noexcept {
  game_manager_.Update();
}

void ClientApplication::Draw() noexcept {
  if (raylib::IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    UnloadRenderTexture(render_texture_);
    render_texture_ = raylib::LoadRenderTexture(new_tex_size.X, new_tex_size.Y);
  }

  game_renderer_.Draw(render_texture_);

  raylib::DrawTextureRec(render_texture_.texture,
      raylib::Rectangle{0, 0, static_cast<float>(render_texture_.texture.width),
                        static_cast<float>(-render_texture_.texture.height)},
      raylib::Vector2{0.f, 0.f}, raylib::kWhite);
}

void ClientApplication::DrawImGui() noexcept {}

void ClientApplication::TearDown() noexcept {
  game_manager_.Deinit();
  game_renderer_.Deinit();
}
