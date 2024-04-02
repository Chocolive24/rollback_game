#include <imgui.h>
#include <imgui_impl_raylib.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

class Renderer {
public:
  void Init() noexcept;
  void Draw() noexcept;
  void Deinit() noexcept;
};

void DrawMainFrame(void* renderer) noexcept {
  static_cast<Renderer*>(renderer)->Draw();
}

void Renderer::Draw() noexcept {
 BeginDrawing(); {
   ClearBackground(BLACK);

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

void Renderer::Deinit() noexcept {
 // Cleanup
 ImGui_ImplRaylib_Shutdown();
 ImGui::DestroyContext();
}

void Renderer::Init() noexcept {
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

// Main code
int main(int, char**) {
  // Setup raylib window
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
  InitWindow(1280, 720, "Dear ImGui Raylib(OpenGL) example");

  Renderer renderer;  
  renderer.Init();

// If emscripten is called, let the web browser decided the target fps and give the
// main loop function to emscripten.
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(DrawMainFrame, &renderer, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    ImGui_ImplRaylib_ProcessEvents();

   renderer.Draw();
  }

#endif
  renderer.Deinit();

  CloseWindow();

  return 0;
}