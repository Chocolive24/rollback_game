#include "debug_client.h"
#include "engine.h"

void DebugClient::Init(int client_idx,
                       Math::Vec2I render_tex_size) noexcept {
  client_idx_ = client_idx;
  render_texture_ = LoadRenderTexture(render_tex_size.X, render_tex_size.Y);
}

void DebugClient::Update() noexcept {
  inputs_ = game::GetPlayerInputs(client_idx_);

  if (IsWindowResized()) {
    auto new_tex_size = Engine::window_size();
    new_tex_size.X /= 2;
    render_texture_.texture.width = new_tex_size.X;
    render_texture_.texture.height = new_tex_size.Y;
  }
}

void DebugClient::Draw() noexcept {
  BeginTextureMode(render_texture_);
  {
    ClearBackground(BLACK);

    const auto tex_center = Math::Vec2I(render_texture_.texture.width / 2,
                                        render_texture_.texture.height / 2);

    const char* client_idx_txt = TextFormat("Client_%02i", client_idx_);
    DrawText(client_idx_txt, tex_center.X - MeasureText(client_idx_txt, kFontSize) / 2, 
        75.f, kFontSize, WHITE);

    Color key_color =
        inputs_ & static_cast<game::PlayerInputs>(game::PlayerInputTypes::kUp)
            ? WHITE
            : DARKGRAY;
    DrawText("UP", tex_center.X - MeasureText("UP", kFontSize) / 2, 1 * kTextOffsetY, kFontSize, key_color);

    key_color =
        inputs_ & static_cast<game::PlayerInputs>(game::PlayerInputTypes::kLeft)
            ? WHITE
            : DARKGRAY;
    DrawText("LEFT", tex_center.X - MeasureText("LEFT", kFontSize) / 2,
             2 * kTextOffsetY, kFontSize, key_color);

    key_color =
        inputs_ & static_cast<game::PlayerInputs>(game::PlayerInputTypes::kDown)
            ? WHITE
            : DARKGRAY;
    DrawText("DOWN", tex_center.X - MeasureText("DOWN", kFontSize) / 2,
             3 * kTextOffsetY, kFontSize, key_color);

    key_color = inputs_ & static_cast<game::PlayerInputs>(
                              game::PlayerInputTypes::kRight)
                    ? WHITE
                    : DARKGRAY;
    DrawText("RIGHT", tex_center.X - MeasureText("RIGHT", kFontSize) / 2,
             4 * kTextOffsetY, kFontSize, key_color);
  }
  EndTextureMode();
}

void DebugClient::Deinit() noexcept {
  UnloadRenderTexture(render_texture_);
}

void DebugClient::SendPacket(game::PlayerInputs inputs,
                                   DebugClient* receiver) noexcept {
  receiver->ReceivePacket(inputs);
}

game::PlayerInputs DebugClient::ReceivePacket(
    game::PlayerInputs inputs) noexcept {
  return game::PlayerInputs();
}