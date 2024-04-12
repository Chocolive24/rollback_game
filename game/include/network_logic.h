#pragma once

#include <Common-cpp/inc/Logger.h>
#include <LoadBalancing-cpp/inc/Listener.h>
#include <LoadBalancing-cpp/inc/Client.h>

class Listener final : public ExitGames::LoadBalancing::Listener {
 public:
  void debugReturn(int debugLevel,
                   const ExitGames::Common::JString& string) override {}
  void connectionErrorReturn(int errorCode) override {
    std::cout << "error connection\n";
  }
  void clientErrorReturn(int errorCode) override {
    std::cout << "client error code : " << errorCode << '\n';
  }
  void warningReturn(int warningCode) override {
    std::cout << "client warning code : " << warningCode << '\n';
  }
  void serverErrorReturn(int errorCode) override {
    std::cout << "server error code : " << errorCode << '\n';
  }
  void joinRoomEventAction(
      int playerNr, const ExitGames::Common::JVector<int>& playernrs,
      const ExitGames::LoadBalancing::Player& player) override {}
  void leaveRoomEventAction(int playerNr, bool isInactive) override {}
  void customEventAction(
      int playerNr, nByte eventCode,
      const ExitGames::Common::Object& eventContent) override {}
  void connectReturn(int errorCode,
                     const ExitGames::Common::JString& errorString,
                     const ExitGames::Common::JString& region,
                     const ExitGames::Common::JString& cluster) override {
    std::cout << "connected, error code:" << errorCode << " "
              << "error string: " << errorString << " "
              << "region: " << region << " "
              << "cluster: " << cluster << '\n';
  }
  void disconnectReturn() override { std::cout << "client disconnected\n"; }
  void leaveRoomReturn(int errorCode,
                       const ExitGames::Common::JString& errorString) override {
  }
};

class SampleNetworkLogic {
 public:
  SampleNetworkLogic(const ExitGames::Common::JString& appID,
                     const ExitGames::Common::JString& appVersion);
  void connect(void);
  void disconnect(void);
  void run(void);

  void SampleNetworkLogic::createRoom(
      const ExitGames::Common::JString& roomName, nByte maxPlayers);

 private:
  ExitGames::LoadBalancing::Client mLoadBalancingClient;
  Listener mListener;  // your implementation of the
                       // ExitGames::LoadBalancing::Listener interface
  ExitGames::Common::Logger mLogger;  // accessed by EGLOG()
};

inline SampleNetworkLogic::SampleNetworkLogic(
    const ExitGames::Common::JString& appID,
    const ExitGames::Common::JString& appVersion)
    : mLoadBalancingClient(mListener, appID, appVersion) {}

inline void SampleNetworkLogic::connect(void) {
  // connect() is asynchronous - the actual result arrives in the
  // Listener::connectReturn() or the Listener::connectionErrorReturn() callback
  std::cout << "hello\n";
  if (!mLoadBalancingClient.connect())
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"Could not connect.");
}

inline void SampleNetworkLogic::run() { mLoadBalancingClient.service(); }

inline void SampleNetworkLogic::disconnect(void) {
  mLoadBalancingClient.disconnect();  // disconnect() is asynchronous - the actual result
                      // arrives in the Listener::disconnectReturn() callback
}

inline void SampleNetworkLogic::createRoom(const ExitGames::Common::JString& roomName,
                                           nByte maxPlayers) {
  mLoadBalancingClient.opCreateRoom(
      roomName,
      ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(maxPlayers));
}