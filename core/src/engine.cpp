#include "engine.h"

#include <raylib.h>
#include <imgui.h>
#include <imgui_impl_raylib.h>

#include "texture_manager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

/**
 * \brief DrawMainFrame is a function declared and passed to emscripten's main loop
 *  when emscripten is called during build. It updates and draws the
 *  current application's frame.
 */
#ifdef __EMSCRIPTEN__
void DrawMainFrame(void* engine) noexcept {
  static_cast<Engine*>(engine)->ProcessFrame();
}
#endif

void Engine::Run() noexcept {
  Setup();

  // If emscripten is called, let the web browser decide the target fps and give
  // the main loop function to emscripten.
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(DrawMainFrame, this, 0, 1);

#else
  while (!WindowShouldClose()) {
    ProcessFrame();
  }

#endif

  TearDown();
}

void Engine::ProcessFrame() {
  ClearBackground(BLACK);

  if (IsWindowResized()) {
    window_size_.X = GetScreenWidth();
    window_size_.Y = GetScreenHeight();
  }

  application_->Update();

  ImGui_ImplRaylib_ProcessEvents();

  BeginDrawing();
  {
    application_->Draw();

    // Start the Dear ImGui frame
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    application_->DrawImGui();

    // Rendering
    ImGui::Render();
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
  }
  EndDrawing();
}

void Engine::Setup() noexcept {
  unsigned config_flags = FLAG_WINDOW_RESIZABLE;

#ifndef __EMSCRIPTEN__
  config_flags |= FLAG_VSYNC_HINT;
#endif

  SetConfigFlags(config_flags);
  InitWindow(window_size_.X, window_size_.Y, "Rollback Game");

  // Setup Dear ImGui context
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplRaylib_Init();

  // required to be called to cache the font texture with raylib
  Imgui_ImplRaylib_BuildFontAtlas();

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  texture_manager::CreateAllSprites();

  application_->Setup();
}

void Engine::TearDown() noexcept {
  application_->TearDown();

  texture_manager::DestroyAllSprites();

  ImGui_ImplRaylib_Shutdown();
  ImGui::DestroyContext();

  CloseWindow();
}