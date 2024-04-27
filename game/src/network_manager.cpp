#include "network_manager.h"
#include "event.h"
#include "client.h"

NetworkManager::NetworkManager(
    const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion)
    : load_balancing_client_(*this, appID, appVersion) {
  FrameInput::registerType();
}

NetworkManager::~NetworkManager() noexcept {
  FrameInput::unregisterType();
}

void NetworkManager::Connect() {
  // Connect() is asynchronous - the actual result arrives in the
  // ClientNetworkManager::connectReturn() or the ClientNetworkManager::connectionErrorReturn() callback
  if (!load_balancing_client_.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not Connect.");
}

void NetworkManager::Service() { load_balancing_client_.service(); }

void NetworkManager::JoinRandomOrCreateRoom() noexcept {
   const auto game_id = ExitGames::Common::JString();
   const ExitGames::LoadBalancing::RoomOptions room_options(
      true, true, 2); 
  if (!load_balancing_client_.opJoinRandomOrCreateRoom(game_id, room_options))
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not join or create room.");
}

void NetworkManager::Disconnect() {
  load_balancing_client_.disconnect();  // Disconnect() is asynchronous - the actual result
                      // arrives in the ClientNetworkManager::disconnectReturn() callback
}

void NetworkManager::CreateRoom(
    const ExitGames::Common::JString& roomName, nByte maxPlayers) {
  if (load_balancing_client_.opCreateRoom(roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {

    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void NetworkManager::JoinRandomRoom(ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  load_balancing_client_.opJoinRandomRoom(expectedCustomRoomProperties);

  //is_master_ = load_balancing_client_.getLocalPlayer().getIsMasterClient();
  //std::cout << "is master: " << is_master_ << '\n';
}

void NetworkManager::RaiseEvent(bool reliable,
                                      NetworkEventCode event_code,
                                      const ExitGames::Common::Hashtable& event_data) noexcept {
  if (!load_balancing_client_.opRaiseEvent(reliable, event_data,
                                      static_cast<nByte>(event_code))) {
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not raise event.");
  }
}

void NetworkManager::ReceiveEvent(int player_nr, NetworkEventCode event_code,
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

  if (network_game_manager_ == nullptr) 
      return;

  const NetworkEvent network_event{event_code, event_content};
  network_game_manager_->PushNetworkEvent(network_event);
  //network_game_manager_->OnEventReceived(event_code, event_content);

  //switch (event_code) {
  //  case EventCode::kInput: {
  //    break;
  //  }
  //  default: {
  //    // have a look at demo_typeSupport inside the C++ client SDKs for
  //    // example code on how to send and receive more fancy data types
  //  } break;
  //}
}


void NetworkManager::debugReturn(int debugLevel,
                           const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void NetworkManager::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
}

void NetworkManager::clientErrorReturn(int errorCode) {
  std::cout << "client error code : " << errorCode << '\n';
}

void NetworkManager::warningReturn(int warningCode) {
  std::cout << "client warning code : " << warningCode << '\n';
}

void NetworkManager::serverErrorReturn(int errorCode) {
  std::cout << "server error code : " << errorCode << '\n';
}

void NetworkManager::joinRoomEventAction(
    int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) {
  std::cout << "Room state: player nr: " << playerNr
            << " player nrs size: " << playernrs.getSize() << " player userID: "
            << player.getUserID().UTF8Representation().cstr() << '\n';

  if (client_ == nullptr) return;

  if (client_->client_id() == game_constants::kInvalidClientId)
  {
    client_->SetClientId(playerNr);
    client_->SetState(ClientState::kInRoom);
  }


  if (playerNr == game_constants::kMaxPlayerCount) {
    client_->StartGame();
  }
}

void NetworkManager::leaveRoomEventAction(int playerNr, bool isInactive) {
  std::cout << "player nr: " << playerNr << " is inactive: " << isInactive
            << '\n';
}

void NetworkManager::customEventAction(int playerNr, nByte eventCode,
    const ExitGames::Common::Object& eventContent) {
  if (eventContent.getType() != ExitGames::Common::TypeCode::HASHTABLE) {
    std::cerr << "Unsupported event content type \n";
    return;
  }

  const ExitGames::Common::Hashtable& event_data =
      ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContent).getDataCopy();

   ReceiveEvent(playerNr, static_cast<NetworkEventCode>(eventCode), event_data);
}

void NetworkManager::connectReturn(int errorCode,
                             const ExitGames::Common::JString& errorString,
                             const ExitGames::Common::JString& region,
                             const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';

  if (client_ == nullptr) return;

  client_->SetState(ClientState::kInMainMenu);
}

void NetworkManager::disconnectReturn() { std::cout << "client disconnected\n"; }

void NetworkManager::leaveRoomReturn(int errorCode,
                               const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

void NetworkManager::joinRandomOrCreateRoomReturn(
    int i, const ExitGames::Common::Hashtable& hashtable,
    const ExitGames::Common::Hashtable& hashtable1, int i1,
    const ExitGames::Common::JString& string) {
  ExitGames::LoadBalancing::Listener::joinRandomOrCreateRoomReturn(
      i, hashtable, hashtable1, i1, string);
  std::cout << "Joined or created a room\n";
}
