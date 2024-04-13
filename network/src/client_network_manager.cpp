#include "client_network_manager.h"
#include "event.h"

ClientNetworkManager::ClientNetworkManager(
    const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion)
    : load_balancing_client_(listener_, appID, appVersion) {}

void ClientNetworkManager::Connect() {
  // Connect() is asynchronous - the actual result arrives in the
  // Listener::connectReturn() or the Listener::connectionErrorReturn() callback
  if (!load_balancing_client_.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not Connect.");
}

void ClientNetworkManager::Service() { load_balancing_client_.service(); }

void ClientNetworkManager::JoinRandomOrCreateRoom() noexcept {
   const auto game_id = ExitGames::Common::JString();
   const ExitGames::LoadBalancing::RoomOptions room_options(
      true, true, 2); 
  if (!load_balancing_client_.opJoinRandomOrCreateRoom(game_id, room_options))
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not join or create room.");
}

void ClientNetworkManager::Disconnect() {
  load_balancing_client_.disconnect();  // Disconnect() is asynchronous - the actual result
                      // arrives in the Listener::disconnectReturn() callback
}

void ClientNetworkManager::CreateRoom(
    const ExitGames::Common::JString& roomName, nByte maxPlayers) {
  if (load_balancing_client_.opCreateRoom(roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {

    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void ClientNetworkManager::JoinRandomRoom(ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  load_balancing_client_.opJoinRandomRoom(expectedCustomRoomProperties);

  //is_master_ = load_balancing_client_.getLocalPlayer().getIsMasterClient();
  //std::cout << "is master: " << is_master_ << '\n';
}

void ClientNetworkManager::RaiseEvent(bool reliable,
                                      EventCode event_code,
                                      const ExitGames::Common::Hashtable& event_data) noexcept {
  if (!load_balancing_client_.opRaiseEvent(reliable, event_data,
                                      static_cast<nByte>(event_code))) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not raise event.");
  }
}

void ClientNetworkManager::ReceiveEvent(int player_nr, EventCode event_code,
                                        const ExitGames::Common::Hashtable& event_content) noexcept {

   // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive
  EGLOG(ExitGames::Common::DebugLevel::ALL,
        L"an event of type %d from player Nr %d with the following content has "
        L"just arrived: %ls",
        static_cast<nByte>(event_code), player_nr,
        event_content.toString(true).cstr());

  std::cout << "event content: "
            << event_content.toString().UTF8Representation().cstr() << '\n';

  switch (event_code) {
    case EventCode::kInput: {
      break;
    }
    default: {
      // have a look at demo_typeSupport inside the C++ client SDKs for
      // example code on how to send and receive more fancy data types
    } break;
  }
}
