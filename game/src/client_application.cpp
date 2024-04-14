#include "client_application.h"

void ClientApplication::Setup() noexcept {
  game_manager_.Init();
  game_renderer_.Init();
}

void ClientApplication::Update() noexcept {
  game_manager_.Update();
}

void ClientApplication::Draw() noexcept {
  game_renderer_.Draw();
}

void ClientApplication::DrawImGui() noexcept {}

void ClientApplication::TearDown() noexcept {
  game_manager_.Deinit();
  game_renderer_.Deinit();
}
