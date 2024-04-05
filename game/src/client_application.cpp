#include "client_application.h"

void ClientApplication::Setup() noexcept { game_renderer_.Init(); }

void ClientApplication::Update() noexcept { game_renderer_.Update(); }

void ClientApplication::Draw() noexcept { game_renderer_.Draw(); }

void ClientApplication::TearDown() noexcept { game_renderer_.Deinit(); }
