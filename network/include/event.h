#pragma once

#include <Common-cpp/inc/defines.h>

enum class EventCode : nByte {
  kInput = 0,
  kLeft,
  kRight,
  kJump,
  kShoot
};

enum class EventKey : nByte {
  kPlayerInput = 0,
  kFrameNbr,
  kDelay,
};