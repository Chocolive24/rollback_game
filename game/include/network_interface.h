#pragma once

#include "event.h"

/**
 * \brief The NetworkInterface class defines an interface for events
 * communication over the network.
 *
 * This interface can be implemented by classes that perform actual network
 * communication over a network protocol, as well as by mock implementations
 * that simulate data exchange for testing or other purposes. By using this
 * interface, applications can easily switch between real and mock
 * implementations without changing the code that interacts with the
 * communication module.
 */
class NetworkInterface {
public:
  constexpr explicit NetworkInterface() noexcept = default;
  explicit NetworkInterface(NetworkInterface&& other) noexcept = default;
  NetworkInterface& operator=(NetworkInterface&& other) noexcept = default;
  explicit NetworkInterface(const NetworkInterface& other) noexcept = default;
  NetworkInterface& operator=(const NetworkInterface& other) noexcept = default;
  virtual ~NetworkInterface() noexcept = default;

  virtual void JoinRandomOrCreateRoom() noexcept = 0;
  virtual void LeaveRoom() noexcept = 0;

  /**
   * \brief RaiseEvent is a method which raises an event over the network.
   *
   * This method is responsible for sending an event, along with the provided
   * data, over the network. The reliability of the event transmission may be
   * specified using the 'reliable' parameter.
   *
   * \param reliable Indicates whether the event should be sent reliably.
   * \param event_data The data to be sent as part of the event.
   * \param event_code An identifier specifying the type or purpose of the
   * event.
   */
  virtual void RaiseEvent(bool reliable,
                          NetworkEventCode event_code,
                          const ExitGames::Common::Hashtable& event_data) noexcept = 0;

  /**
   * \brief Receives and processes an incoming event from the network.
   *
   * This method is responsible for handling incoming events received over the
   * network. It processes the event content and takes appropriate actions based
   * on the event type and data.
   *
   * \param player_nr      The player number or identifier associated with the
   * received event.
   * \param event_code     An identifier specifying the type or purpose of the
   * received event.
   * \param event_content  The data or payload of the received event.
   */
  virtual void ReceiveEvent(int player_nr, NetworkEventCode event_code,
                            const ExitGames::Common::Hashtable& event_content) noexcept = 0;
};
