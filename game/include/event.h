#pragma once

#include <Common-cpp/inc/defines.h>
#include <Common-cpp/inc/Containers/Hashtable.h>

/**
 * \brief EventCode is an enum which differentiates between the various
 * events in the application.
 */
enum class NetworkEventCode : nByte {
  kInput = 0,
  kFrameConfirmation
};

/**
 * \brief EventKey is an enum which differentiates between the various
 * keys used to serialize/deserialize the hashtable events.
 */
enum class NetworkEventKey : nByte {
  kPlayerInput = 0,
  kFrameNbr,
  kDelay,
  kCheckSum
};

struct NetworkEvent {
  NetworkEventCode code{};
  ExitGames::Common::Hashtable content{};
};