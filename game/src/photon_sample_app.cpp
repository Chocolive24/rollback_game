#include "photon_sample_app.h"
#include "texture_manager.h"

#include "event.h"

#include <imgui.h>

void PhotonSampleApp::Setup() noexcept {
  texture_manager::CreateAllSprites();
}

void PhotonSampleApp::Update() noexcept {
  networkLogic_.Service();
}

void PhotonSampleApp::Draw() noexcept {
  raylib::DrawRectangle(700, 500, 100, 100, raylib::Color{255, 0, 0, 255});

  raylib::DrawRaylibText("Raylib drawing text with photon in the same app.", 
      400, 350, 30, raylib::kWhite);

  texture_manager::penguin.Draw({600, 600});
}

void PhotonSampleApp::DrawImGui() noexcept {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

  ImGui::Begin("Network UI"); {
    if (ImGui::Button("Connect", ImVec2(125, 25))) {
      networkLogic_.Connect();
    }

    ImGui::Spacing();

    if(ImGui::Button("Disconnect", ImVec2(125, 25))) {
      networkLogic_.Disconnect();
    }

    ImGui::Spacing();

    if (ImGui::Button("Join or create Room", ImVec2(125, 25))) {
      networkLogic_.JoinRandomOrCreateRoom();
    }

    ImGui::Spacing();

    if (ImGui::Button("Join Room", ImVec2(125, 25))) {
      networkLogic_.JoinRandomRoom();
    }

    ImGui::Spacing();

    if (ImGui::Button("Send event", ImVec2(125, 25))) {
      ExitGames::Common::Hashtable evData;  
      evData.put<nByte, int>(static_cast<nByte>(EventKey::kPlayerInput),42);
      constexpr bool sendReliable = false; 
      networkLogic_.RaiseEvent(sendReliable, EventCode::kInput, evData);
    }
  }
  ImGui::End();
}

void PhotonSampleApp::TearDown() noexcept {
  networkLogic_.Disconnect();
}
