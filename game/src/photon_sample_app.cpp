#include "photon_sample_app.h"
#include "raylib_wrapper.h"

#include <imgui.h>

void PhotonSampleApp::Setup() noexcept {

}

void PhotonSampleApp::Update() noexcept {
  networkLogic_.run();
}

void PhotonSampleApp::Draw() noexcept {
  raylib::DrawRectangle(700, 500, 100, 100, raylib::Color{255, 0, 0, 255});

  raylib::DrawRaylibText("Raylib drawing text with photon in the same app.", 
      400, 350, 30, raylib::kWhite);
}

void PhotonSampleApp::DrawImGui() noexcept {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin("Network UI"); {
    if (ImGui::Button("Connect", ImVec2(125, 25))) {
      networkLogic_.connect();
    }

    ImGui::Spacing();

    if(ImGui::Button("Disconnect", ImVec2(125, 25))) {
      networkLogic_.disconnect();
    }

    ImGui::Spacing();

    if (ImGui::Button("Create Room", ImVec2(125, 25))) {
      networkLogic_.createRoom("MyRoom", 2);
    }

    ImGui::Spacing();

    if (ImGui::Button("Join Room", ImVec2(125, 25))) {
      networkLogic_.JoinRandomRoom();
    }
  }
  ImGui::End();
}

void PhotonSampleApp::TearDown() noexcept {
  networkLogic_.disconnect();
}
