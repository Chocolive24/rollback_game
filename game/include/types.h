#pragma once

#include <cstdint>

using FrameNbr = std::int16_t;
using PlayerId = std::int8_t;
using ClientId = std::int8_t;

///**
// * \brief ClientId
// */
//class ClientId {
// public:
//  explicit ClientId(std::int8_t id) noexcept;
//
//  // TODO operator de conversion vers int.
//
// private:
//  std::int8_t id_ = 0;
//};