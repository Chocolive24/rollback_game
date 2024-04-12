#include "photon_sample_app.h"
#include "raylib_cpp.h"

#include <imgui.h>

void PhotonSampleApp::Setup() noexcept {

}

void PhotonSampleApp::Update() noexcept { networkLogic_.run(); }

void PhotonSampleApp::Draw() noexcept {
  raylib::DrawRectangle(500, 500, 100, 100, raylib::kRed);
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
  }
  ImGui::End();
}

void PhotonSampleApp::TearDown() noexcept { networkLogic_.disconnect(); }
