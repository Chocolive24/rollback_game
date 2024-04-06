#include "Photon-cpp/inc/LitePeer.h"

#include <iostream>

class Listener final : public ExitGames::Photon::PhotonListener {
public:
  void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {
   //std::cout << "debug return " << debugLevel << " " << string << '\n';
  }
  ~Listener() override = default;
  void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse) override {
   if (operationResponse.getReturnCode() == ExitGames::Photon::StatusCode::CONNECT) {
     std::cout << "Joined room successfully." << std::endl;
   } else if (operationResponse.getReturnCode() ==
              ExitGames::Photon::StatusCode::DISCONNECT) {
     std::cout << "Left room successfully." << std::endl;
   }
   // Handle other operation responses as needed
  }
  void onStatusChanged(int statusCode) override {
   std::cout << "onStatusChanged : " << statusCode << '\n';
  }
  void onEvent(const ExitGames::Photon::EventData& eventData) override {
   std::cout << "onEvent " << eventData.toString() << '\n';
  }
  void onMessage(const ExitGames::Common::Object&) override{}
  void onRawMessage(nByte*, int) override{}
  void onPingResponse(const ExitGames::Common::JString& ping, unsigned) override {
   std::cout << ping << '\n';
  }
};

int main() {
  Listener listener;
  ExitGames::Lite::LitePeer peer(listener);

  if (!peer.connect("127.0.0.1:5055"))
  {
    std::cerr << "Could not connect to server\n";
  }

  std::cout << "connected\n";

  while (true)
  {
    peer.opRaiseEvent(true, "Hello", 1);
    //peer.service();
    //peer.pingServer("127.0.0.1 : 5055", 10);
  }

  //// Main loop
  //for (int i = 0; i < 100; ++i) {
  // // Regularly call service() to handle events and send operations
  // peer.service();

  // peer.pingServer("localhost", 10);

  // // Send data (event) in the game
  // peer.opRaiseEvent(true, "Hello, world!", 1);

  // // Handle other game logic as needed

  // // Sleep or delay
  // // This is just for demonstration purposes, in a real application,
  // // you may have some other logic or input handling here.
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //}

  peer.disconnect();

  return EXIT_SUCCESS;
}