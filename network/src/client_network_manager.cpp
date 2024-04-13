#include "client_network_manager.h"
#include "event.h"

ClientNetworkManager::ClientNetworkManager(
    const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion)
    : load_balancing_client_(*this, appID, appVersion) {}

void ClientNetworkManager::Connect() {
  // Connect() is asynchronous - the actual result arrives in the
  // ClientNetworkManager::connectReturn() or the ClientNetworkManager::connectionErrorReturn() callback
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
                      // arrives in the ClientNetworkManager::disconnectReturn() callback
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


void ClientNetworkManager::debugReturn(int debugLevel,
                           const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void ClientNetworkManager::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
}

void ClientNetworkManager::clientErrorReturn(int errorCode) {
  std::cout << "client error code : " << errorCode << '\n';
}

void ClientNetworkManager::warningReturn(int warningCode) {
  std::cout << "client warning code : " << warningCode << '\n';
}

void ClientNetworkManager::serverErrorReturn(int errorCode) {
  std::cout << "server error code : " << errorCode << '\n';
}

void ClientNetworkManager::joinRoomEventAction(
    int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) {
  std::cout << "Room state: player nr: " << playerNr
            << " player nrs size: " << playernrs.getSize() << " player userID: "
            << player.getUserID().UTF8Representation().cstr() << '\n';
}

void ClientNetworkManager::leaveRoomEventAction(int playerNr, bool isInactive) {
  std::cout << "player nr: " << playerNr << " is inactive: " << isInactive
            << '\n';
}

void ClientNetworkManager::customEventAction(
    int playerNr, nByte eventCode,
    const ExitGames::Common::Object& eventContent) {
  if (eventContent.getType() != ExitGames::Common::TypeCode::HASHTABLE) {
    std::cerr << "Unsupported event content type \n";
    return;
  }

  const ExitGames::Common::Hashtable& event_data =
      ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent)
          .getDataCopy();

   ReceiveEvent(playerNr, static_cast<EventCode>(eventCode), event_data);
}

void ClientNetworkManager::connectReturn(int errorCode,
                             const ExitGames::Common::JString& errorString,
                             const ExitGames::Common::JString& region,
                             const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';
}

void ClientNetworkManager::disconnectReturn() { std::cout << "client disconnected\n"; }

void ClientNetworkManager::leaveRoomReturn(int errorCode,
                               const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

void ClientNetworkManager::joinRandomOrCreateRoomReturn(
    int i, const ExitGames::Common::Hashtable& hashtable,
    const ExitGames::Common::Hashtable& hashtable1, int i1,
    const ExitGames::Common::JString& string) {
  ExitGames::LoadBalancing::Listener::joinRandomOrCreateRoomReturn(
      i, hashtable, hashtable1, i1, string);
  std::cout << "Joined or created a room\n";
}
