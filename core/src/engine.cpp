#include "engine.h"

#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

/**
 * \brief DrawMainFrame is a function declared and passed to emscripten's main loop
 *  when emscripten is called during build. It updates and draws the
 *  current application's frame.
 */
#ifdef __EMSCRIPTEN__
void DrawMainFrame(void* app) noexcept {
  const auto application = static_cast<Application*>(app);
  application->Update();
  application->Draw();
}
#endif

void Engine::Run() noexcept {
  Setup();

  // If emscripten is called, let the web browser decide the target fps and give
  // the main loop function to emscripten.
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(DrawMainFrame, application_, 0, 1);

#else
  //SetTargetFPS(60);

  while (!WindowShouldClose()) {
    application_->Update();
    application_->Draw();
  }

#endif

  TearDown();
}

void Engine::Setup() noexcept {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow(1280, 720, "Rollback Game");

  application_->Setup();
}

void Engine::TearDown() noexcept {
  application_->TearDown();
  CloseWindow();
}
