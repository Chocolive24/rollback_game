#include "photon_sample_app.h"
#include "texture_manager.h"

#include "event.h"

#include <imgui.h>

void PhotonSampleApp::Setup() noexcept {
  texture_manager::CreateAllSprites();
}

void PhotonSampleApp::Update() noexcept {
  networkLogic_.Service();

  ExitGames::Common::Hashtable evData;
  std::vector<input::FrameInput> fs{};

  input::FrameInput frame_input(Math::Vec2F::One(), 42, 2);

  fs.push_back(frame_input);
  fs.push_back(frame_input);
  fs.push_back(frame_input);

  evData.put<nByte, input::FrameInput*>(
      static_cast<nByte>(NetworkEventKey::kPlayerInput), 
      fs.data(), fs.size());

  input::FrameInput* f;
  int count = 0;

  if (evData.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput))) {
     const auto input_value = evData.getValue(static_cast<nByte>(NetworkEventKey::kPlayerInput));
    f = ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
            .getDataCopy();
     count = *ExitGames::Common::ValueObject<input::FrameInput*>(input_value)
                 .getSizes();
  }

  std::vector<input::FrameInput> fsss{};

  for (int i = 0; i < count; i++)
  {
     fsss.push_back(f[i]);
     
  }

 for (auto s : fsss)
 {
     std::cout << s.frame_nbr() << '\n';
     std::cout << s.input() << '\n';
     std::cout << s.dir_to_mouse().X << '\n';
 }

  constexpr bool sendReliable = false;
  networkLogic_.RaiseEvent(sendReliable, NetworkEventCode::kInput, evData);
}

void PhotonSampleApp::Draw() noexcept {
  raylib::DrawRectangle(700, 500, 100, 100, raylib::Color{255, 0, 0, 255});

  raylib::DrawRaylibText("Raylib drawing text with photon in the same app.", 
      400, 350, 30, raylib::kWhite);

  texture_manager::penguin_blue.Draw({600, 600});
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
      evData.put<nByte, int>(static_cast<nByte>(NetworkEventKey::kPlayerInput),42);

      constexpr bool sendReliable = false; 
      networkLogic_.RaiseEvent(sendReliable, NetworkEventCode::kInput, evData);
    }
  }
  ImGui::End();
}

void PhotonSampleApp::TearDown() noexcept {
  networkLogic_.Disconnect();
}
