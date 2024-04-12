#pragma once

#include "listener.h"

#include <LoadBalancing-cpp/inc/Client.h>
#include <Common-cpp/inc/Logger.h>

class NetworkManager final : public NetworkInterface {
 public:
  NetworkManager(const ExitGames::Common::JString& appID,
                 const ExitGames::Common::JString& appVersion);
  void connect();
  void disconnect();
  void run();

  void createRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);

  void JoinRandomRoom(ExitGames::Common::Hashtable expectedCustomRoomProperties =
          ExitGames::Common::Hashtable());
  void RaiseEvent(bool reliable, const ExitGames::Common::Hashtable& data, 
      nByte event_code) noexcept override;
  void ReceiveEvent(int player_nr, nByte event_code, 
      const ExitGames::Common::Object& event_content) noexcept override;

private:
  ExitGames::LoadBalancing::Client load_balancing_client_;
  Listener listener_{this};
  ExitGames::Common::Logger mLogger;  // name must be mLogger because it is accessed by EGLOG()
};