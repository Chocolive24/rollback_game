#include "debug_client.h"
#include "engine.h"

void DebugInputsClient::Init(int client_idx,
                             Math::Vec2I render_tex_size) noexcept {
  client_idx_ = client_idx;
  render_texture_ = LoadRenderTexture(render_tex_size.X, render_tex_size.Y);
}

void DebugInputsClient::Update() noexcept {
  inputs_ = game::GetPlayerInputs(client_idx_);

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
    Color key_color =
        inputs_ & static_cast<game::PlayerInputs>(game::PlayerInputTypes::kUp)
            ? WHITE
            : DARKGRAY;
    DrawText("W", tex_center.X, 1 * kTextOffsetY, 45, key_color);

    key_color =
        inputs_ & static_cast<game::PlayerInputs>(game::PlayerInputTypes::kLeft)
            ? WHITE
            : DARKGRAY;
    DrawText("A", tex_center.X, 2 * kTextOffsetY, 45, key_color);

    key_color =
        inputs_ & static_cast<game::PlayerInputs>(game::PlayerInputTypes::kDown)
            ? WHITE
            : DARKGRAY;
    DrawText("S", tex_center.X, 3 * kTextOffsetY, 45, key_color);

    key_color = inputs_ & static_cast<game::PlayerInputs>(
                              game::PlayerInputTypes::kRight)
                    ? WHITE
                    : DARKGRAY;
    DrawText("D", tex_center.X, 4 * kTextOffsetY, 45, key_color);
  }
  EndTextureMode();
}

void DebugInputsClient::Deinit() noexcept {
  UnloadRenderTexture(render_texture_);
}

void DebugInputsClient::SendPacket(game::PlayerInputs inputs,
                                   DebugInputsClient* receiver) noexcept {
  receiver->ReceivePacket(inputs);
}

game::PlayerInputs DebugInputsClient::ReceivePacket(
    game::PlayerInputs inputs) noexcept {
  return game::PlayerInputs();
}