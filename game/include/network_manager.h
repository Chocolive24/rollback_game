#pragma once

#include "network_interface.h"

#include <Common-cpp/inc/Logger.h>
#include <LoadBalancing-cpp/inc/Client.h>

#include "online_game_manager.h"

class Client;

/**
 * \brief ClientNetworkManager is a class responsible for managing network
 * communication on the client side using Photon's LoadBalancing API.
 *
 * It implements the NetworkInterface and acts as a listener
 * for various network events such as connection, disconnection, errors, room
 * joins, and custom events.
 */
class NetworkManager final : public NetworkInterface,
                             private ExitGames::LoadBalancing::Listener {
 public:
  /**
   * \brief Constructor for the ClientNetworkManager class.
   * \param appID The application ID used for connecting to the Photon server.
   * \param appVersion The application version used for connecting to the Photon
   * server.
   */
  NetworkManager(const ExitGames::Common::JString& appID,
                       const ExitGames::Common::JString& appVersion);
  NetworkManager(NetworkManager&& other) noexcept = delete;
  NetworkManager& operator=(NetworkManager&& other) noexcept = delete;
  NetworkManager(const NetworkManager& other) noexcept = delete;
  NetworkManager& operator=(const NetworkManager& other) noexcept = delete;
  ~NetworkManager() noexcept override;

  void RegisterClient(Client* client) noexcept {
    client_ = client;
  }

  void RegisterOnlineGameManager(OnlineGameManager* network_game_manager) noexcept {
    online_game_manager_ = network_game_manager;
  }

  /**
   * \brief Initiates a connection to the Photon server.
   */
  void Connect();

  /**
   * \brief Disconnects from the Photon server.
   */
  void Disconnect();

  /**
   * \brief Performs service operations required for network communication.
   */
  void Service();

  /**
   * \brief Joins a random room or creates a new room if no rooms are available.
   */
  void JoinRandomOrCreateRoom() noexcept;

  /**
   * \brief Creates a new room with the specified name and maximum number of
   * players. \param roomName The name of the room to be created. \param
   * maxPlayers The maximum number of players allowed in the room.
   */
  void CreateRoom(const ExitGames::Common::JString& roomName, nByte maxPlayers);

  /**
   * \brief Joins a random room with optional filtering based on custom room
   * properties. \param expectedCustomRoomProperties The expected custom room
   * properties to filter rooms.
   */
  void JoinRandomRoom(ExitGames::Common::Hashtable expectedCustomRoomProperties =
          ExitGames::Common::Hashtable());

  /**
   * \brief Raises a custom event to be sent to other players in the room.
   * \param reliable Indicates whether the event should be sent reliably or not.
   * \param event_code The code representing the type of event being raised.
   * \param event_data The data associated with the event.
   */
  void RaiseEvent(bool reliable, NetworkEventCode event_code,
      const ExitGames::Common::Hashtable& event_data) noexcept override;

  /**
   * \brief Receives and handles custom events sent by other players in the
   * room. \param player_nr The player number who sent the event. \param
   * event_code The code representing the type of event received. \param
   * event_content The data associated with the received event.
   */
  void ReceiveEvent(int player_nr, NetworkEventCode event_code,
                    const ExitGames::Common::Hashtable& event_content) noexcept override;


 private:
  // Methods overridden from ExitGames::LoadBalancing::Listener
  // These methods handle various network events such as connection,
  // disconnection, errors, room joins, and custom events.
  // ===============================================================

  void debugReturn(int debugLevel,
                   const ExitGames::Common::JString& string) override;

  void connectionErrorReturn(int errorCode) override;
  void clientErrorReturn(int errorCode) override;
  void warningReturn(int warningCode) override;
  void serverErrorReturn(int errorCode) override;

  void joinRoomEventAction(
      int playerNr, const ExitGames::Common::JVector<int>& playernrs,
      const ExitGames::LoadBalancing::Player& player) override;

  void leaveRoomEventAction(int playerNr, bool isInactive) override;

  void customEventAction(
      int playerNr, nByte eventCode,
      const ExitGames::Common::Object& eventContent) override;

  void connectReturn(int errorCode,
                     const ExitGames::Common::JString& errorString,
                     const ExitGames::Common::JString& region,
                     const ExitGames::Common::JString& cluster) override;

  void disconnectReturn() override;

  void leaveRoomReturn(int errorCode,
                       const ExitGames::Common::JString& errorString) override;
  void joinRandomOrCreateRoomReturn(int, const ExitGames::Common::Hashtable&,
                                    const ExitGames::Common::Hashtable&, int,
                                    const ExitGames::Common::JString&) override;

  // Member variables.
  // =================
  Client* client_ = nullptr;
  OnlineGameManager* online_game_manager_ = nullptr;
  ExitGames::LoadBalancing::Client load_balancing_client_;
  ExitGames::Common::Logger mLogger;  // name must be mLogger because it is accessed by EGLOG()
};