#include "listener.h"

#include "LoadBalancing-cpp/inc/Player.h"

Listener::Listener(NetworkInterface* network) noexcept: network_(network) {}

void Listener::debugReturn(int debugLevel,
                           const ExitGames::Common::JString& string) {
  std::cout << "debug return: debug level: " << debugLevel
            << " string: " << string.UTF8Representation().cstr() << '\n';
}

void Listener::connectionErrorReturn(int errorCode) {
  std::cout << "error connection\n";
}

void Listener::clientErrorReturn(int errorCode) {
  std::cout << "client error code : " << errorCode << '\n';
}

void Listener::warningReturn(int warningCode) {
  std::cout << "client warning code : " << warningCode << '\n';
}

void Listener::serverErrorReturn(int errorCode) {
  std::cout << "server error code : " << errorCode << '\n';
}

void Listener::joinRoomEventAction(
    int playerNr, const ExitGames::Common::JVector<int>& playernrs,
    const ExitGames::LoadBalancing::Player& player) {
  std::cout << "Room state: player nr: " << playerNr
            << " player nrs size: " << playernrs.getSize() << " player userID: "
            << player.getUserID().UTF8Representation().cstr() << '\n';
}

void Listener::leaveRoomEventAction(int playerNr, bool isInactive) {
  std::cout << "player nr: " << playerNr << " is inactive: " << isInactive
            << '\n';
}

void Listener::customEventAction(int playerNr, nByte eventCode,
    const ExitGames::Common::Object& eventContent) {
  if (network_ != nullptr)
    network_->ReceiveEvent(playerNr, static_cast<EventCode>(eventCode), eventContent);
}

void Listener::connectReturn(int errorCode,
                             const ExitGames::Common::JString& errorString,
                             const ExitGames::Common::JString& region,
                             const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';
}

void Listener::disconnectReturn() { std::cout << "client disconnected\n"; }

void Listener::leaveRoomReturn(int errorCode,
                               const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

void Listener::joinRandomOrCreateRoomReturn(int i, const ExitGames::Common::Hashtable& hashtable,
  const ExitGames::Common::Hashtable& hashtable1, int i1, const ExitGames::Common::JString& string) {
  ExitGames::LoadBalancing::Listener::joinRandomOrCreateRoomReturn(i, hashtable, hashtable1, i1, string);
  std::cout << "Joined or created a room\n";
}
