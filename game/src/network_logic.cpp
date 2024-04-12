#include "network_logic.h"

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
}

void Listener::connectReturn(
    int errorCode, const ExitGames::Common::JString& errorString,
                   const ExitGames::Common::JString& region,
                   const ExitGames::Common::JString& cluster) {
  std::cout << "connected, error code:" << errorCode << " "
            << "error string: " << errorString.UTF8Representation().cstr()
            << " "
            << "region: " << region.UTF8Representation().cstr() << " "
            << "cluster: " << cluster.UTF8Representation().cstr() << '\n';
}

void Listener::disconnectReturn() {
  std::cout << "client disconnected\n";
}

void Listener::leaveRoomReturn(int errorCode,
                     const ExitGames::Common::JString& errorString) {
  std::cout << "Leave room return: error code: " << errorCode
            << " error string: " << errorString.UTF8Representation().cstr()
            << '\n';
}

SampleNetworkLogic::SampleNetworkLogic(const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion) :
mLoadBalancingClient(mListener, appID, appVersion) {
  
}

void SampleNetworkLogic::connect() {
  // connect() is asynchronous - the actual result arrives in the
  // Listener::connectReturn() or the Listener::connectionErrorReturn() callback
  std::cout << "hello\n";
  if (!mLoadBalancingClient.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

void SampleNetworkLogic::run() {
  mLoadBalancingClient.service();
}

void SampleNetworkLogic::disconnect() {
  mLoadBalancingClient
      .disconnect();  // disconnect() is asynchronous - the actual result
                      // arrives in the Listener::disconnectReturn() callback
}

void SampleNetworkLogic::createRoom(const ExitGames::Common::JString& roomName, 
    nByte maxPlayers) {
  if (mLoadBalancingClient.opCreateRoom(
          roomName,
          ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers))) {
    std::cout << "Room successfully created with name: "
              << roomName.UTF8Representation().cstr() << '\n';
  }
}

void SampleNetworkLogic::JoinRandomRoom(
    ExitGames::Common::Hashtable expectedCustomRoomProperties) {
  mLoadBalancingClient.opJoinRandomRoom(expectedCustomRoomProperties);
}