#pragma once

#include <Common-cpp/inc/Logger.h>
#include <LoadBalancing-cpp/inc/Listener.h>
#include <LoadBalancing-cpp/inc/Client.h>

class Listener final : public ExitGames::LoadBalancing::Listener {
 public:
  void debugReturn(int debugLevel,
                   const ExitGames::Common::JString& string) override;
  void connectionErrorReturn(int errorCode) override;
  void clientErrorReturn(int errorCode) override;
  void warningReturn(int warningCode) override;
  void serverErrorReturn(int errorCode) override;
  void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs,
      const ExitGames::LoadBalancing::Player& player) override;
  void leaveRoomEventAction(int playerNr, bool isInactive) override;
  void customEventAction(int playerNr, nByte eventCode,
      const ExitGames::Common::Object& eventContent) override;
  void connectReturn(int errorCode,
                     const ExitGames::Common::JString& errorString,
                     const ExitGames::Common::JString& region,
                     const ExitGames::Common::JString& cluster) override;

  void disconnectReturn() override;

  void leaveRoomReturn(int errorCode,
                       const ExitGames::Common::JString& errorString) override;
private:
  ExitGames::Common::Logger* mLogger = nullptr;
};

class SampleNetworkLogic {
 public:
  SampleNetworkLogic(const ExitGames::Common::JString& appID,
                     const ExitGames::Common::JString& appVersion);
  void connect();
  void disconnect();
  void run();

  void createRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);

  void JoinRandomRoom(ExitGames::Common::Hashtable expectedCustomRoomProperties =
          ExitGames::Common::Hashtable());
  void RaiseEvent(bool priv, const ExitGames::Common::Hashtable& hashtable, nByte byte);
  void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

private:
  ExitGames::LoadBalancing::Client load_balancing_client_;
  Listener listener_;
  ExitGames::Common::Logger mLogger;  // name must be mLogger because it is accessed by EGLOG()
};