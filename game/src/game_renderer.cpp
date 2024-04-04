#include "game_renderer.h"
#include "Metrics.h"

#include <raylib.h>
#include <imgui.h>
#include <imgui_impl_raylib.h>

#include <iostream>

void GameRenderer::Init() noexcept {
  GameManager::Init();

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
}

void GameRenderer::Draw() noexcept {
  // As ImGui is purely used in a graphic context, I consider it correct to call
  // the ImGui events process in the draw method.
  ImGui_ImplRaylib_ProcessEvents();

  const auto& body_ref = world_.GetCollider(player_controller_.col_ref_).GetBodyRef();
  const auto& body = world_.GetBody(body_ref);
  auto ball_pos = body.Position();
  ball_pos = Metrics::MetersToPixels(ball_pos);

  BeginDrawing(); {
    ClearBackground(BLACK);

    DrawCircle(ball_pos.X, ball_pos.Y, Metrics::MetersToPixels(
        game_constants::kPlayerColRadius), RED);

    // Start the Dear ImGui frame
   ImGui_ImplRaylib_NewFrame();
   ImGui::NewFrame();
   
   ImGui::ShowDemoWindow();
   
   // Rendering
   ImGui::Render();
   ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
  }
  EndDrawing();
}

void GameRenderer::Deinit() noexcept {
  GameManager::Deinit();

  ImGui_ImplRaylib_Shutdown();
  ImGui::DestroyContext();
}
