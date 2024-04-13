#pragma once

#include "listener.h"

#include <LoadBalancing-cpp/inc/Client.h>
#include <Common-cpp/inc/Logger.h>

class ClientNetworkManager final : public NetworkInterface {
 public:
  ClientNetworkManager(const ExitGames::Common::JString& appID,
                 const ExitGames::Common::JString& appVersion);
  void Connect();
  void Disconnect();
  void Service();

  void JoinRandomOrCreateRoom() noexcept;

  void CreateRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);

  void JoinRandomRoom(ExitGames::Common::Hashtable expectedCustomRoomProperties =
          ExitGames::Common::Hashtable());
  void RaiseEvent(bool reliable, const ExitGames::Common::Hashtable& data,
                  EventCode event_code) noexcept override;
  void ReceiveEvent(int player_nr, EventCode event_code,
                    const ExitGames::Common::Object& event_content) noexcept override;

  void SetIsMaster(const bool is_master) noexcept { is_master_ = is_master; }

private:
  ExitGames::LoadBalancing::Client load_balancing_client_;
  Listener listener_{this};
  ExitGames::Common::Logger mLogger;  // name must be mLogger because it is accessed by EGLOG()
  bool is_master_ = false;
};