#pragma once

#include <Common-cpp/inc/defines.h>

/**
 * \brief EventCode is an enum which differentiates between the various
 * events in the application.
 */
enum class EventCode : nByte {
  kInput = 0,
};

/**
 * \brief EventKey is an enum which differentiates between the various
 * keys used to serialize/deserialize the hashtable events.
 */
enum class EventKey : nByte {
  kPlayerInput = 0,
  kFrameNbr,
  kDelay,
};