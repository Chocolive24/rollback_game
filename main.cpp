#include <imgui_impl_raylib.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <iostream>

#include "game_renderer.h"

void DrawMainFrame(void* renderer) noexcept {
  const auto game_renderer = static_cast<GameRenderer*>(renderer);
  game_renderer->PollInputs();
  game_renderer->Update();
  game_renderer->Draw();
}

int main() {
  // Init raylib window.
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "Rollback Game");

  // Init game renderer.
  GameRenderer game_renderer;  
  game_renderer.Init();

  // If emscripten is called, let the web browser decided the target fps and give the
  // main loop function to emscripten.
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(DrawMainFrame, &renderer, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    game_renderer.PollInputs();
    game_renderer.Update();
    game_renderer.Draw();
  }

#endif
  game_renderer.Deinit();

  CloseWindow();

  return 0;
}