#include "photon_sample_app.h"
#include "texture_manager.h"

#include <imgui.h>

void PhotonSampleApp::Setup() noexcept {
  texture_manager::CreateAllSprites();
}

void PhotonSampleApp::Update() noexcept {
  networkLogic_.run();
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

    ImGui::Spacing();

    if (ImGui::Button("Send event", ImVec2(125, 25))) {
      nByte eventCode = 1;  // use distinct event codes to distinguish between different types
              // of events (for example 'move', 'shoot', etc.)
      ExitGames::Common::Hashtable evData;  // organize your payload data in any way you like as long as
                   // it is supported by Photons serialization
      evData.put(L"message",42);
      bool sendReliable = false;  // send something reliable if it has to arrive everywhere
      networkLogic_.RaiseEvent(sendReliable, evData, eventCode);
    }
  }
  ImGui::End();
}

void PhotonSampleApp::TearDown() noexcept {
  networkLogic_.disconnect();
}
