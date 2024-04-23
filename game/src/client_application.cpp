#include "client_application.h"

#include "engine.h"

void ClientApplication::Setup() noexcept {
  game_manager_.Init(0, 0);
  game_manager_.RegisterRollbackManager(&rollback_manager_);
  game_renderer_.Init();

  network_manager_.Connect();

  render_texture_ = raylib::LoadRenderTexture(raylib::GetScreenWidth(),
                                              raylib::GetScreenHeight());
}

void ClientApplication::Update() noexcept {
  //static FrameNbr current_frame = -1;
  //current_frame++;
  //game_manager_.FixedUpdate(current_frame);

  network_manager_.Service();
}

void ClientApplication::Draw() noexcept {
  if (raylib::IsWindowResized()) {
    const auto new_tex_size = Engine::window_size();
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

  network_manager_.Disconnect();
}
