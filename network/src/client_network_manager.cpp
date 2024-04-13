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
                                      const ExitGames::Common::Hashtable& data,
                                      EventCode event_code) noexcept {
  if (!load_balancing_client_.opRaiseEvent(reliable, data,
                                      static_cast<nByte>(event_code))) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not raise event.");
  }
}

void ClientNetworkManager::ReceiveEvent(int player_nr, EventCode event_code,
    const ExitGames::Common::Object& event_content) noexcept {
  // logging the string representation of the eventContent can be really useful
  // for debugging, but use with care: for big events this might get expensive
  switch (event_code) {
    case EventCode::kJump: {
      // you can access the content as a copy (might be a bit expensive for
      // really big data constructs)
      ExitGames::Common::Hashtable content =
          ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(
              event_content).getDataCopy();

      // Print the content of the hashtable as a whole
      std::cout << "content: "
                << content.toString(true).UTF8Representation().cstr() << '\n';

      // Access the content by key
      const ExitGames::Common::Object* value = content.getValue(
          static_cast<nByte>(EventKey::kJump));

      if (value != nullptr) {
        std::cout << "content by key: "
                  << value->toString(true).UTF8Representation().cstr() << '\n';
      }
      else {
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not get the event content by key.");
      }
      break;
    }
    default: {
      // have a look at demo_typeSupport inside the C++ client SDKs for
      // example code on how to send and receive more fancy data types
    } break;
  }
}
